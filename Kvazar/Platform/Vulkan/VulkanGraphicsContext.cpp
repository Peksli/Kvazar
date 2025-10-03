#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"
#include "Platform/Vulkan/VMA.h"

#include "VkBootstrap.h"


namespace Kvazar {

	VulkanContext* VulkanContext::m_Context = nullptr;

	VulkanContext::VulkanContext(const ContextSpec& spec)
	{
		KVAZAR_DEBUG("[Vulkan context] VulkanContext(const ContextSpec& spec) called");
		m_Context = this;

		m_ContextData.m_Window = spec.m_Window;
	}

	VulkanContext::~VulkanContext()
	{
		KVAZAR_DEBUG("[Vulkan context] ~VulkanContext() called");
	}

	void VulkanContext::Setup()
	{
		// Instance 
		vkb::InstanceBuilder builder;

		vkb::Instance vkb_instance = builder
			.set_app_name("Vulkan engine")
			.request_validation_layers(true)
			.use_default_debug_messenger()
			.require_api_version(1, 3, 0)
			.build()
			.value();

		m_ContextData.m_Instance = vkb_instance.instance;
		if (!vkb_instance)
		{
			KVAZAR_CRITICAL("[Instance] Didn't create instance!!!");
		}
		else
		{
			KVAZAR_DEBUG("[Instance] Creating instance...");
		}

		// Debug messenger
		m_ContextData.m_DebugMessenger = vkb_instance.debug_messenger;
		if (m_ContextData.m_DebugMessenger == VK_NULL_HANDLE)
		{
			KVAZAR_CRITICAL("[Debug messenger] Didn't create debug messenger!!!");
		}
		else
		{
			KVAZAR_DEBUG("[Debug messenger] Creating debug messenger...");
		}

		// Surface
		glfwCreateWindowSurface(m_ContextData.m_Instance, m_ContextData.m_Window, nullptr, &m_ContextData.m_Surface);

		if (m_ContextData.m_Surface == VK_NULL_HANDLE)
		{
			KVAZAR_CRITICAL("[Surface] Didn't create surface!!!");
		}
		else
		{
			KVAZAR_DEBUG("[Surface] Creating surface...");
		}

		// Physical device
		VkPhysicalDeviceVulkan13Features features13{ };
		features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		features13.dynamicRendering = true;
		features13.synchronization2 = true;

		VkPhysicalDeviceVulkan12Features features12{ };
		features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		features12.bufferDeviceAddress = true;
		features12.descriptorIndexing = true;

		vkb::PhysicalDeviceSelector phDeviceSelector{ vkb_instance };
		vkb::PhysicalDevice phDevice = phDeviceSelector
			.set_minimum_version(1, 3)
			.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
			.set_required_features_13(features13)
			.set_required_features_12(features12)
			.set_surface(m_ContextData.m_Surface)
			.select()
			.value();

		if (!phDevice)
		{
			KVAZAR_CRITICAL("[Physical device] Didn't choose physical device!!!");
		}
		else
		{
			KVAZAR_DEBUG("[Physical device] Selecting physical device...");
			m_ContextData.m_PhysicalDevice = phDevice.physical_device;
		}

		// Logical device 
		vkb::DeviceBuilder deviceBuilder{ phDevice };
		vkb::Device vkbDevice = deviceBuilder.build().value();

		if (!vkbDevice)
		{
			KVAZAR_CRITICAL("[Physical device] Didn't create logical device!!!");
		}
		else
		{
			KVAZAR_DEBUG("[Logical device] Creating logical device...");

			VulkanLogicalDeviceBuilder deviceBuilder;
			m_ContextData.m_LogicalDevice = deviceBuilder
				.Reset()
				.SetDevice(vkbDevice.device)
				.SetGraphicsQueue(vkbDevice.get_queue(vkb::QueueType::graphics).value())
				.SetPresentationQueue(vkbDevice.get_queue(vkb::QueueType::present).value())
				.SetGraphicsIndex(vkbDevice.get_queue_index(vkb::QueueType::graphics).value())
				.SetPresentIndex(vkbDevice.get_queue_index(vkb::QueueType::present).value())
				.Build();
		}

		// Swapchain
		int width, height;
		glfwGetFramebufferSize(m_ContextData.m_Window, &width, &height);

		vkb::SwapchainBuilder swapchainBuilder
		{ 
			m_ContextData.m_PhysicalDevice, 
			m_ContextData.m_LogicalDevice.GetDevice(),
			m_ContextData.m_Surface 
		};

		vkb::Swapchain vkbSwapchain = swapchainBuilder
			.set_desired_format(VkSurfaceFormatKHR{ 
			.format = VK_FORMAT_R8G8B8A8_UNORM, 
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent(width, height)
			.set_desired_min_image_count(2)
			.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT 
			| VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
			.build()
			.value();

		if (!vkbSwapchain)
		{ 
			KVAZAR_CRITICAL("[Swapchain] Didn't create swapchain!!!"); 
		}
		else
		{ 
			KVAZAR_DEBUG("[Swapchain] Creating swapchain...");

			VulkanSwapchainBuilder vulkanSwapchainBuilder;
			m_ContextData.m_Swapchain = vulkanSwapchainBuilder
				.SetNextImageIndex(0)
				.SetRawSwapchain(vkbSwapchain.swapchain)
				.SetImagesFormat(vkbSwapchain.image_format)
				.SetImagesExtent({ vkbSwapchain.extent.width, vkbSwapchain.extent.height, 1 })
				.SetImages(std::move(vkbSwapchain.get_images().value()))
				.SetImageViews(std::move(vkbSwapchain.get_image_views().value()))
				.Build();
		}
	}

	void VulkanContext::Shutdown()
	{
		// Swapchain
		m_ContextData.m_Swapchain.Shutdown();

		// Surface
		KVAZAR_DEBUG("[Surface] Destroying surface...");
		vkDestroySurfaceKHR(m_ContextData.m_Instance, m_ContextData.m_Surface, nullptr);

		// Logical device
		vkDestroyDevice(m_ContextData.m_LogicalDevice.GetDevice(), nullptr);

		// Debug messenger
		KVAZAR_DEBUG("[Debug messenger] Destroying debug messenger...");
		vkb::destroy_debug_utils_messenger(m_ContextData.m_Instance, m_ContextData.m_DebugMessenger);

		// instance
		KVAZAR_DEBUG("[Instance] Destroying instance...");
		vkDestroyInstance(m_ContextData.m_Instance, nullptr);
	}

}