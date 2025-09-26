#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(x)													\
do {																\
	VkResult err = x;												\
	if (err)														\
	{																\
		KVAZAR_CRITICAL(FMT_STRING("Critical: {0}"), string_VkResult(err));		\
	}																\
} while(0)															\

#include "Platform/Vulkan/VulkanCommandBuffer.h"



namespace Kvazar {

	namespace Utils {

		void TransitionImageLayout(
			VulkanCommandBuffer& commandBuffer,
			VkImage					image,
			VkPipelineStageFlags2	srcStageMask,
			VkAccessFlags2			srcAccessMask,
			VkPipelineStageFlags2	dstStageMask,
			VkAccessFlags2			dstAccessMask,
			VkImageLayout			oldLayout,
			VkImageLayout			newLayout
		);

	}

}