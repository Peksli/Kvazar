#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanDebug.h"
#include "Core/LogSystem.h"

#include "VkBootstrap.h"


namespace Kvazar {

//	VulkanGraphicsContextBuilder* VulkanGraphicsContextBuilder::s_VulkanContextBuilder;
//
//	VulkanGraphicsContextBuilder::VulkanGraphicsContextBuilder(const GraphicsContextSpecification& specification)
//		: m_Instance(VK_NULL_HANDLE), m_ContextSpecification(specification)
//	{
//		s_VulkanContextBuilder = this;
//		KVAZAR_DEBUG("[Vulkan Graphics Context Builder] VulkanGraphicsContextBuilder() called\n");
//	}
//
//	VulkanGraphicsContextBuilder::~VulkanGraphicsContextBuilder()
//	{
//		KVAZAR_DEBUG("[Vulkan Graphics Context Builder] ~VulkanGraphicsContextBuilder() called\n");
//	}
//
//	void VulkanGraphicsContextBuilder::Build()
//	{
//		BuildInstance();
//		BuildDebugger();
//		BuildSurface();
//		BuildPhysicalDevice();
//		BuildLogicalDevice();
//		BuildSwapChain();
//	}
//
//	void VulkanGraphicsContextBuilder::Destroy()
//	{
//		DestroySwapChain();
//		DestroySurface();
//		DestroyLogicalDevice();
//		DestroyPhysicalDevice();
//		DestroyDebugger();
//		DestroyInstance();
//	}
//
//	void VulkanGraphicsContextBuilder::BuildInstance()
//	{
//		m_DebugMessenger = std::make_shared<VulkanDebugMessenger>();
//
//		vkb::InstanceBuilder builder;
//
//		auto inst_res = builder.set_app_name("Vulkan engine")
//			.request_validation_layers(true)
//			.use_default_debug_messenger()
//			.require_api_version(1, 4, 0)
//			.build();
//
//		vkb::Instance vkb_instance = inst_res.value();
//
//		m_Instance = vkb_instance.instance;
//		m_DebugMessenger->SetMessenger(vkb_instance.debug_messenger);
//	}
//
//	void VulkanGraphicsContextBuilder::DestroyInstance()
//	{
//		if (m_Instance != VK_NULL_HANDLE)
//		{
//			KVAZAR_DEBUG("Destroying instance!");
//			vkDestroyInstance(m_Instance, nullptr);
//		}
//		else
//		{
//			KVAZAR_DEBUG("Can not destroy instance, it is alreade VK_NULL_HANDLE!");
//		}
//	}
//
//	void VulkanGraphicsContextBuilder::BuildDebugger()
//	{
//		//m_VulkanDebugger = std::make_shared<VulkanDebugger>();
//		//if (m_VulkanDebugger->Create() != VK_SUCCESS)
//		//{
//		//	KVAZAR_CRITICAL("Didn't create VulkanDebugger!!!");
//		//}
//		//else
//		//{
//		//	KVAZAR_DEBUG("VulkanDebugger created!!!");
//		//}
//	}
//
//	void VulkanGraphicsContextBuilder::DestroyDebugger()
//	{
//		m_VulkanDebugger->Shutdown();
//		m_VulkanDebugger.reset();
//	}
//
//	void VulkanGraphicsContextBuilder::BuildPhysicalDevice()
//	{
//		VkPhysicalDeviceVulkan12Features features12{};
//		features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
//		features12.bufferDeviceAddress = true;
//		features12.descriptorIndexing = true;
//
//		VkPhysicalDeviceVulkan13Features features13{};
//		features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
//		features13.dynamicRendering = true;
//		features13.synchronization2 = true;
//
//		vkb::PhysicalDeviceSelector selector 
//		//m_PhysicalDevice = VulkanPhysicalDeviceSelector::Select();
//		//if (m_PhysicalDevice->GetRaw() != VK_NULL_HANDLE)
//		//{
//		//	KVAZAR_DEBUG("VulkanPhysicalDevice created!!!");
//		//}
//	}
//
//	void VulkanGraphicsContextBuilder::DestroyPhysicalDevice()
//	{
//		m_PhysicalDevice.reset();
//	}
//
//	void VulkanGraphicsContextBuilder::BuildLogicalDevice()
//	{
//		m_LogicalDevice = VulkanLogicalDeviceCreator::CreateFrom(m_PhysicalDevice);
//		if (m_LogicalDevice->GetRaw() != VK_NULL_HANDLE)
//		{
//			KVAZAR_DEBUG("VulkanLogicalDevice created!!!");
//		}
//	}
//
//	void VulkanGraphicsContextBuilder::DestroyLogicalDevice()
//	{
//		vkDestroyDevice(m_LogicalDevice->GetRaw(), nullptr);
//		m_LogicalDevice.reset();
//	}
//	
//	void VulkanGraphicsContextBuilder::BuildSwapChain()
//	{
//		m_Swapchain = std::make_shared<VulkanSwapchain>();
//		m_Swapchain->Create();
//		if (m_Swapchain->GetRaw() != VK_NULL_HANDLE)
//		{
//			KVAZAR_DEBUG("VulkanSwapchain created!!!");
//		}
//	}
//
//	void VulkanGraphicsContextBuilder::DestroySwapChain()
//	{
//		m_Swapchain->Shutdown();
//		m_Swapchain.reset();
//	}
//	
//
//	void VulkanGraphicsContextBuilder::BuildSurface()
//	{
//		m_Surface = std::make_shared<VulkanSurface>();
//		m_Surface->Create();
//		if (m_Surface->GetRaw() != VK_NULL_HANDLE)
//		{
//			KVAZAR_DEBUG("VulkanSurface created!!!");
//		}
//	}
//
//	void VulkanGraphicsContextBuilder::DestroySurface()
//	{
//		m_Surface->Shutdown();
//		m_Surface.reset();
//	}
//
//	std::vector<const char*> VulkanGraphicsContextBuilder::GetExtensions()
//	{
//		uint32_t extensionCount = 0;
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//		std::vector<VkExtensionProperties> allExtensions(extensionCount);
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allExtensions.data());
//
//		KVAZAR_INFO("\033[5m\033[32m \t> All available instance extensions < \033[0m");
//		for (const auto& availableExtension : allExtensions)
//		{
//			KVAZAR_INFO(FMT_STRING(">>> {0}"), availableExtension.extensionName);
//		}
//
//		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
//		std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + extensionCount);
//
//#ifdef KVAZAR_BUILD_DEBUG
//		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//#endif
//
//		KVAZAR_INFO("\033[5m\033[32m \t> Usable instance extensions < \033[0m");
//		for (const auto& extension : requiredExtensions)
//		{
//			KVAZAR_INFO(FMT_STRING(">>> {0}"), extension);
//		}
//
//		return requiredExtensions;
//	}
//
//	std::vector<const char*> VulkanGraphicsContextBuilder::GetLayers()
//	{
//		uint32_t layerCount = 0;
//		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//		std::vector<VkLayerProperties> allLayers(layerCount);
//		vkEnumerateInstanceLayerProperties(&layerCount, allLayers.data());
//
//		KVAZAR_INFO("\033[5m\033[32m \t> All available layers < \033[0m");
//		for (const auto& layer : allLayers)
//		{
//			KVAZAR_INFO(FMT_STRING(">>> {0}"), layer.layerName);
//		}
//
//		KVAZAR_INFO("\033[5m\033[32m \t> Searching for layers < \033[0m");
//		std::vector<const char*> requiredLayers = { "VK_LAYER_KHRONOS_validation" };
//		for (const auto& reqLayer : requiredLayers)
//		{
//			auto it = std::find_if(allLayers.begin(), allLayers.end(),
//				[&reqLayer](const VkLayerProperties& availableLayer)
//				{
//					return strcmp(reqLayer, availableLayer.layerName) == 0;
//				});
//			if (it == allLayers.end())
//			{
//				KVAZAR_CRITICAL(FMT_STRING("Didn't find layer: {0}"), reqLayer);
//			}
//			else
//			{
//				KVAZAR_INFO(FMT_STRING("Found layer: {0}"), reqLayer);
//			}
//		}
//
//		KVAZAR_INFO("\033[5m\033[32m \t> Validation layers work < \033[0m");
//
//		return requiredLayers;
//	}

	VulkanContext* VulkanContext::m_Context = nullptr;

	VulkanContext::VulkanContext(const ContextSpec& spec)
	{
		m_Context = this;

		m_ContextData.m_Window = spec.m_Window;
	}

	VulkanContext::~VulkanContext()
	{

	}

	void VulkanContext::Setup()
	{
		// Instance 
		vkb::InstanceBuilder builder;

		auto instanceRes = builder
			.set_app_name("Vulkan engine")
			.request_validation_layers(true)
			.use_default_debug_messenger()
			.require_api_version(1, 4, 0)
			.build();

		vkb::Instance vkb_instance = instanceRes.value();
		m_ContextData.m_Instance = vkb_instance.instance;
		if (!vkb_instance) { KVAZAR_CRITICAL("Didn't create instance!!!"); }

		// Debug messenger
		m_ContextData.m_DebugMessenger = vkb_instance.debug_messenger;

		// Surface
		glfwCreateWindowSurface(m_ContextData.m_Instance, m_ContextData.m_Window, nullptr, &m_ContextData.m_Surface);
		if(!m_ContextData.m_Surface) { KVAZAR_CRITICAL("Didn't create surface!!!"); }

		// Physical and logical device
		VkPhysicalDeviceVulkan13Features features13{ };
		features13.sType			= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		features13.dynamicRendering = true;
		features13.synchronization2 = true;

		VkPhysicalDeviceVulkan12Features features12{ };
		features12.sType				= VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		features12.bufferDeviceAddress	= true;
		features12.descriptorIndexing	= true;

		vkb::PhysicalDeviceSelector phDeviceSelector{ vkb_instance };
		vkb::PhysicalDevice phDevice = phDeviceSelector
			.set_minimum_version(1, 2)
			.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
			.set_name("NVIDIA GeForce GTX 1650") // BY DEFAULT IT CHOOSES INTEGRATED GPU, BE CAREFULL THERE !!!!!
			.set_required_features_13(features13)
			.set_required_features_12(features12)
			.set_surface(m_ContextData.m_Surface)
			.select()
			.value();
		if (!phDevice) { KVAZAR_CRITICAL("Didn't choose physical device!!!"); }

		vkb::DeviceBuilder deviceBuilder{ phDevice };
		vkb::Device vkbDevice = deviceBuilder.build().value();	
		if (!vkbDevice) { KVAZAR_CRITICAL("Didn't create logical device!!!"); }

		m_ContextData.m_LogicalDevice.SetDevice(vkbDevice.device);
		m_ContextData.m_PhysicalDevice = phDevice.physical_device;

		// Swapchain
		int width, height;
		glfwGetFramebufferSize(m_ContextData.m_Window, &width, &height);

		vkb::SwapchainBuilder swapchainBuilder
		{ 
			m_ContextData.m_PhysicalDevice, 
			m_ContextData.m_LogicalDevice.GetDevice(),
			m_ContextData.m_Surface 
		};
		m_ContextData.m_Swapchain.SetFormat(VK_FORMAT_R8G8B8A8_UNORM); // maybe won't be hardcoded

		vkb::Swapchain vkbSwapchain = swapchainBuilder
			.set_desired_format(VkSurfaceFormatKHR{ 
			.format = m_ContextData.m_Swapchain.GetFormat(), 
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent(width, height)
			.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			.build()
			.value();

		m_ContextData.m_Swapchain.SetExtent2D(vkbSwapchain.extent);
		m_ContextData.m_Swapchain.SetRaw(vkbSwapchain.swapchain);
		m_ContextData.m_Swapchain.SetImages(vkbSwapchain.get_images().value());
		m_ContextData.m_Swapchain.SetImageViews(vkbSwapchain.get_image_views().value());
	}

	void VulkanContext::Shutdown()
	{
		// Swapchain
		vkDestroySwapchainKHR(m_ContextData.m_LogicalDevice.GetDevice(), m_ContextData.m_Swapchain.GetRaw(), nullptr);
		for (int i = 0; i < m_ContextData.m_Swapchain.GetImages().size(); i++)
		{
			vkDestroyImageView(
				m_ContextData.m_LogicalDevice.GetDevice(),
				m_ContextData.m_Swapchain.GetImageViews()[i],
				nullptr);
		}

		// Surface
		vkDestroySurfaceKHR(m_ContextData.m_Instance, m_ContextData.m_Surface, nullptr);

		// Logical device
		vkDestroyDevice(m_ContextData.m_LogicalDevice.GetDevice(), nullptr);

		// Debug messenger
		vkb::destroy_debug_utils_messenger(m_ContextData.m_Instance, m_ContextData.m_DebugMessenger);

		// instance
		vkDestroyInstance(m_ContextData.m_Instance, nullptr);
	}

}