#pragma once

#include "Platform/Vulkan/VulkanSwapChain.h"

#include <vulkan/vulkan.h>
#include <optional>
#include <cstdint>


namespace Kvazar {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsQueueFamily;
		std::optional<uint32_t> presentationQueueFamily;

		bool IsComplete() { return graphicsQueueFamily.has_value() && presentationQueueFamily.has_value(); }
	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice();
		virtual ~VulkanPhysicalDevice();

		VkPhysicalDevice GetRaw() const { return m_PhysicalDevice; }

	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	};

}