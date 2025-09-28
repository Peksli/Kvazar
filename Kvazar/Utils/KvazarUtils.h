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

		// Barriers

		void TransitionImageLayout(
			VulkanCommandBuffer&	commandBuffer,
			VkImage					image,
			VkPipelineStageFlags2	srcStageMask,
			VkAccessFlags2			srcAccessMask,
			VkPipelineStageFlags2	dstStageMask,
			VkAccessFlags2			dstAccessMask,
			VkImageLayout			oldLayout,
			VkImageLayout			newLayout
		);

		// Images and image views

		VkImageCreateInfo CreateImageInfo(
			VkFormat				format,
			VkExtent3D				imageExtent,
			uint32_t				mipLevels,
			uint32_t				arrayLayers,
			VkSampleCountFlagBits	samples,
			VkImageTiling			tiling,
			VkImageUsageFlags		usageMask
		);

		VkImageViewCreateInfo CreateImageViewInfo(
			VkFormat				format,
			VkImage					image,
			VkImageAspectFlagBits	aspectMask
		);

		// Copy

		void BlitImageToImage(
			VulkanCommandBuffer&	commandBuffer,
			VkImage					srcImage,
			VkImage					dstImage,
			VkExtent3D				srcExtent,
			VkExtent3D				dstExtent
		);

	}

}