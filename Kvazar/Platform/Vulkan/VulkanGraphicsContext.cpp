#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanDebug.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	VulkanGraphicsContextBuilder* VulkanGraphicsContextBuilder::s_VulkanContextBuilder;

	VulkanGraphicsContextBuilder::VulkanGraphicsContextBuilder(const GraphicsContextSpecification& specification)
		: instance(VK_NULL_HANDLE), m_ContextSpecification(specification)
	{
		s_VulkanContextBuilder = this;
		KVAZAR_DEBUG("[Vulkan Graphics Context Builder] VulkanGraphicsContextBuilder() called\n");
	}

	VulkanGraphicsContextBuilder::~VulkanGraphicsContextBuilder()
	{
		KVAZAR_DEBUG("[Vulkan Graphics Context Builder] ~VulkanGraphicsContextBuilder() called\n");
	}

	void VulkanGraphicsContextBuilder::Build()
	{
		BuildInstance();
		BuildDebugger();
		BuildSurface();
		BuildPhysicalDevice();
		BuildLogicalDevice();
		BuildSwapChain();
	}

	void VulkanGraphicsContextBuilder::Destroy()
	{
		DestroySwapChain();
		DestroySurface();
		DestroyLogicalDevice();
		DestroyPhysicalDevice();
		DestroyDebugger();
		DestroyInstance();
	}

	void VulkanGraphicsContextBuilder::BuildInstance()
	{
		VkApplicationInfo applicationCreateInfo = {};
		applicationCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationCreateInfo.pApplicationName = "Kvazar";
		applicationCreateInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationCreateInfo.pEngineName = nullptr;
		applicationCreateInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationCreateInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationCreateInfo;
		std::vector<const char*> extensions = std::move(GetExtensions());
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
		std::vector<const char*> layers = std::move(GetLayers());
#ifdef KVAZAR_BUILD_DEBUG
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = VulkanDebugger::GetDebugMessengerCreateInfo();
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		instanceCreateInfo.ppEnabledLayerNames = layers.data();
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerCreateInfo;
#else
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
		{
			KVAZAR_CRITICAL("Didn't create instance!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroyInstance()
	{
		if (instance != VK_NULL_HANDLE)
		{
			KVAZAR_DEBUG("Destroying instance!");
			vkDestroyInstance(instance, nullptr);
		}
		else
		{
			KVAZAR_DEBUG("Can not destroy instance, it is alreade VK_NULL_HANDLE!");
		}
	}

	void VulkanGraphicsContextBuilder::BuildDebugger()
	{
		m_VulkanDebugger = std::make_shared<VulkanDebugger>();
		if (m_VulkanDebugger->Create() != VK_SUCCESS)
		{
			KVAZAR_CRITICAL("Didn't create VulkanDebugger!!!");
		}
		else
		{
			KVAZAR_DEBUG("VulkanDebugger created!!!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroyDebugger()
	{
		m_VulkanDebugger->Shutdown();
		m_VulkanDebugger.reset();
	}

	void VulkanGraphicsContextBuilder::BuildPhysicalDevice()
	{
		m_PhysicalDevice = VulkanPhysicalDeviceSelector::Select();
		if (m_PhysicalDevice->GetRaw() != VK_NULL_HANDLE)
		{
			KVAZAR_DEBUG("VulkanPhysicalDevice created!!!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroyPhysicalDevice()
	{
		m_PhysicalDevice.reset();
	}

	void VulkanGraphicsContextBuilder::BuildLogicalDevice()
	{
		m_LogicalDevice = VulkanLogicalDeviceCreator::CreateFrom(m_PhysicalDevice);
		if (m_LogicalDevice->GetRaw() != VK_NULL_HANDLE)
		{
			KVAZAR_DEBUG("VulkanLogicalDevice created!!!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroyLogicalDevice()
	{
		vkDestroyDevice(m_LogicalDevice->GetRaw(), nullptr);
		m_LogicalDevice.reset();
	}
	
	void VulkanGraphicsContextBuilder::BuildSwapChain()
	{
		m_Swapchain = std::make_shared<VulkanSwapchain>();
		m_Swapchain->Create();
		if (m_Swapchain->GetRaw() != VK_NULL_HANDLE)
		{
			KVAZAR_DEBUG("VulkanSwapchain created!!!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroySwapChain()
	{
		m_Swapchain->Shutdown();
		m_Swapchain.reset();
	}
	

	void VulkanGraphicsContextBuilder::BuildSurface()
	{
		m_Surface = std::make_shared<VulkanSurface>();
		m_Surface->Create();
		if (m_Surface->GetRaw() != VK_NULL_HANDLE)
		{
			KVAZAR_DEBUG("VulkanSurface created!!!");
		}
	}

	void VulkanGraphicsContextBuilder::DestroySurface()
	{
		m_Surface->Shutdown();
		m_Surface.reset();
	}

	std::vector<const char*> VulkanGraphicsContextBuilder::GetExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> allExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allExtensions.data());

		KVAZAR_INFO("\033[5m\033[32m \t> All available extensions < \033[0m");
		for (const auto& availableExtension : allExtensions)
		{
			KVAZAR_INFO(FMT_STRING(">>> {0}"), availableExtension.extensionName);
		}

		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
		std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + extensionCount);

#ifdef KVAZAR_BUILD_DEBUG
		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		KVAZAR_INFO("\033[5m\033[32m \t> Usable extensions < \033[0m");
		for (const auto& extension : requiredExtensions)
		{
			KVAZAR_INFO(FMT_STRING(">>> {0}"), extension);
		}

		return requiredExtensions;
	}

	std::vector<const char*> VulkanGraphicsContextBuilder::GetLayers()
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> allLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, allLayers.data());

		KVAZAR_INFO("\033[5m\033[32m \t> All available layers < \033[0m");
		for (const auto& layer : allLayers)
		{
			KVAZAR_INFO(FMT_STRING(">>> {0}"), layer.layerName);
		}

		KVAZAR_INFO("\033[5m\033[32m \t> Searching for layers < \033[0m");
		std::vector<const char*> requiredLayers = { "VK_LAYER_KHRONOS_validation" };
		for (const auto& reqLayer : requiredLayers)
		{
			auto it = std::find_if(allLayers.begin(), allLayers.end(),
				[&reqLayer](const VkLayerProperties& availableLayer)
				{
					return strcmp(reqLayer, availableLayer.layerName) == 0;
				});
			if (it == allLayers.end())
			{
				KVAZAR_CRITICAL(FMT_STRING("Didn't find layer: {0}"), reqLayer);
			}
			else
			{
				KVAZAR_INFO(FMT_STRING("Found layer: {0}"), reqLayer);
			}
		}

		KVAZAR_INFO("\033[5m\033[32m \t> Validation layers work < \033[0m");

		return requiredLayers;
	}

}