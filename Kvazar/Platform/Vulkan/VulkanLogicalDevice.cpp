//#include "Platform/Vulkan/VulkanGraphicsContext.h"
//#include "Platform/Vulkan/VulkanLogicalDevice.h"
//#include "Core/LogSystem.h"
//
//
//namespace Kvazar {
//
//	// LOGICAL DEVICE
//
//	VulkanLogicalDevice::VulkanLogicalDevice(VkDevice logicalDevice, VkQueue graphicsQueue, VkQueue presentationQueue)
//		: m_LogicalDevice(logicalDevice), m_GraphicsQueue(graphicsQueue), m_PresentationQueue(presentationQueue)
//	{
//		KVAZAR_DEBUG("[Vulkan Logical Device] VulkanLogicalDevice(VkDevice, VkQueue, VkQueue)\n");
//	}
//
//	VulkanLogicalDevice::~VulkanLogicalDevice()
//	{
//		KVAZAR_DEBUG("[Vulkan Logical Device] ~VulkanLogicalDevice()\n");
//	}
//
//	
//	// CREATOR
//
//	VulkanLogicalDeviceCreator::VulkanLogicalDeviceCreator()
//	{
//
//	}
//
//	VulkanLogicalDeviceCreator::~VulkanLogicalDeviceCreator()
//	{
//
//	}
//
//	std::shared_ptr<VulkanLogicalDevice> VulkanLogicalDeviceCreator::CreateFrom(std::shared_ptr<VulkanPhysicalDevice> device)
//	{
//		QueueFamilyIndices indices = VulkanPhysicalDeviceSelector::GetFamilyIndices(device->GetRaw());
//		std::vector<VkDeviceQueueCreateInfo> deviceQueueInfos;
//		std::set<uint32_t> uniqueDeviceQueueFamiliesIndices = { indices.graphicsQueueFamily.value(), indices.presentationQueueFamily.value() };
//
//		float priority = 1.0f;
//		for (const auto& queueFamilyIndex : uniqueDeviceQueueFamiliesIndices)
//		{
//			VkDeviceQueueCreateInfo queueCreateInfo{};
//			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//			queueCreateInfo.queueCount = 1;
//			queueCreateInfo.pQueuePriorities = &priority;
//			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
//			deviceQueueInfos.push_back(std::move(queueCreateInfo));
//		}
//
//		VkPhysicalDeviceFeatures physicalDeviceFeatures{};
//
//		// Extensions
//		uint32_t extensionCount = 0;
//		vkEnumerateDeviceExtensionProperties(device->GetRaw(), nullptr, &extensionCount, nullptr);
//		std::vector<VkExtensionProperties> extensions(extensionCount);
//		vkEnumerateDeviceExtensionProperties(device->GetRaw(), nullptr, &extensionCount, extensions.data());
//
//		std::set<std::string> exts(requiredExtensions.begin(), requiredExtensions.end());
//		for (const auto& extension : extensions)
//		{
//			exts.erase(extension.extensionName);
//		}
//		if (!exts.empty()) { KVAZAR_CRITICAL("Didn't find required extensions!"); }
//
//		//vulkan 1.2 features
//		VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
//		features12.bufferDeviceAddress = true;
//		features12.descriptorIndexing = true;
//
//		//vulkan 1.3 features
//		VkPhysicalDeviceVulkan13Features features13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
//		features13.pNext = &features12;
//		features13.dynamicRendering = true;
//		features13.synchronization2 = true;
//
//		// Logical device create info
//		VkDeviceCreateInfo logicalDeviceCreateInfo{};
//		logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//		logicalDeviceCreateInfo.pNext = &features13;
//		logicalDeviceCreateInfo.pQueueCreateInfos = deviceQueueInfos.data();
//		logicalDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueInfos.size());
//		logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
//		logicalDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
//		logicalDeviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
//		logicalDeviceCreateInfo.enabledLayerCount = 0;
//		logicalDeviceCreateInfo.ppEnabledLayerNames = nullptr;
//
//		VkDevice logicalDevice;
//		if (vkCreateDevice(device->GetRaw(), &logicalDeviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
//		{
//			KVAZAR_CRITICAL("Didn't create logical device!");
//		}
//
//		VkQueue graphicsQueue;		vkGetDeviceQueue(logicalDevice, indices.graphicsQueueFamily.value(), 0, &graphicsQueue);
//		VkQueue presentationQueue;	vkGetDeviceQueue(logicalDevice, indices.presentationQueueFamily.value(), 0, &presentationQueue);
//
//		return std::make_shared<VulkanLogicalDevice>(logicalDevice, graphicsQueue, presentationQueue);
//	}
//
//	std::vector<const char*> VulkanLogicalDeviceCreator::requiredExtensions = 
//	{ 
//		VK_KHR_SWAPCHAIN_EXTENSION_NAME
//	};
//
//}