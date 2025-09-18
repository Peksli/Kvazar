#include "Platform/Vulkan/VulkanPhysicalDevice.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	// DEVICE

	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice)
		: m_PhysicalDevice(physicalDevice)
	{
		KVAZAR_DEBUG("[Vulkan Physical Device] VulkanPhysicalDevice(VkPhysicalDevice) called");

		m_SwapchainDetails = GetSwapchainDetails();
		if (!m_SwapchainDetails.IsComplete()) { KVAZAR_CRITICAL("Have nothing in m_SwapchainDetails!"); }
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{
		KVAZAR_DEBUG("[Vulkan Physical Device] ~VulkanPhysicalDevice() called");
	}

	SwapchainDetails VulkanPhysicalDevice::GetSwapchainDetails()
	{
		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
		VkSurfaceKHR surface = contextBuilder->GetVulkanSurface()->GetRaw();

		SwapchainDetails details;

		// Surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, surface, &details.m_SurfaceCapabilities);

		// Surface formats
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, nullptr);
		if (formatCount > 0)
		{
			details.m_SurfaceFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, surface, &formatCount, details.m_SurfaceFormats.data());
		}

		// Present modes
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, surface, &presentModeCount, nullptr);
		if (presentModeCount > 0)
		{
			details.m_PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, surface, &presentModeCount, details.m_PresentModes.data());
		}

		return details;
	}


	// SELECTOR

	VulkanPhysicalDeviceSelector::VulkanPhysicalDeviceSelector()
	{
		KVAZAR_DEBUG("VulkanPhysicalDeviceSelector::VulkanPhysicalDeviceSelector() called");
	}

	VulkanPhysicalDeviceSelector::~VulkanPhysicalDeviceSelector()
	{
		KVAZAR_DEBUG("VulkanPhysicalDeviceSelector::~VulkanPhysicalDeviceSelector() called");
	}

	std::shared_ptr<VulkanPhysicalDevice> VulkanPhysicalDeviceSelector::Select()
	{
		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
		VkInstance instance = contextBuilder->GetInstance();

		// Enumerate all physical devices
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// Rate every physical device
		std::multimap<uint32_t, VkPhysicalDevice> candidates;
		uint32_t deviceScore = 0;

		for (const auto& device: devices)
		{
			deviceScore = RatePhysicalDevice(device);
			candidates.insert(std::make_pair(deviceScore, device));
		}
		if (candidates.rbegin()->first > 0)
		{
			return std::make_shared<VulkanPhysicalDevice>(candidates.rbegin()->second);
		}
		else
		{
			KVAZAR_CRITICAL("Didn't find any suitable device!");
			return nullptr;
		}
	}

	QueueFamilyIndices VulkanPhysicalDeviceSelector::GetFamilyIndices(VkPhysicalDevice device)
	{
		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
		VkSurfaceKHR surface = contextBuilder->GetVulkanSurface()->GetRaw();

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		QueueFamilyIndices indices;

		for (uint32_t i = 0; i < queueFamilies.size(); ++i)
		{
			// graphics queue
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsQueueFamily = i;
				continue;
			}

			// presentation queue
			VkBool32 presentationQueueSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationQueueSupport);
			if (presentationQueueSupport)
			{
				indices.presentationQueueFamily = i;
			}
		}

		return indices;
	}

	uint32_t VulkanPhysicalDeviceSelector::RatePhysicalDevice(VkPhysicalDevice device)
	{
		uint32_t deviceScore = 0;

		// Checking properties + features
		VkPhysicalDeviceProperties deviceProperties;	vkGetPhysicalDeviceProperties(device, &deviceProperties);
		VkPhysicalDeviceFeatures deviceFeatures;		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { deviceScore += 100; }
		if (!deviceFeatures.geometryShader) { return 0; }
		deviceScore += deviceProperties.limits.maxImageDimension2D;

		// Checking extensions
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> deviceExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, deviceExtensions.data());

		std::set<std::string> requiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		for (const auto& deviceExtension : deviceExtensions) { requiredExtensions.erase(deviceExtension.extensionName); }

		if (requiredExtensions.empty()) { deviceScore += 30; }
		else { return 0; }

		// Finding queueFamilies ( for graphics + presentation (intended to be separate))
		QueueFamilyIndices indices = GetFamilyIndices(device);
		if (indices.IsComplete()) { deviceScore += 30; }
		else { return 0; }

		return deviceScore;
	}

}