#include "Utils/KvazarUtils.h"


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
			VkImageLayout			newLayout)
		{
			VkImageMemoryBarrier2 imageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
			imageBarrier.pNext = nullptr;

			imageBarrier.srcStageMask = srcStageMask; // first sync scope
			imageBarrier.srcAccessMask = srcAccessMask;
			imageBarrier.dstStageMask = dstStageMask; // second sync scope
			imageBarrier.dstAccessMask = dstAccessMask;

			imageBarrier.oldLayout = oldLayout;
			imageBarrier.newLayout = newLayout;

			VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
				? VK_IMAGE_ASPECT_DEPTH_BIT
				: VK_IMAGE_ASPECT_COLOR_BIT;

			VkImageSubresourceRange subRange = {};
			subRange.aspectMask = aspectMask;
			subRange.baseArrayLayer = 0;
			subRange.layerCount = 1;
			subRange.baseMipLevel = 0;
			subRange.levelCount = 1;

			imageBarrier.subresourceRange = subRange;
			imageBarrier.image = image;

			VkDependencyInfo depInfo{};
			depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
			depInfo.pNext = nullptr;

			depInfo.imageMemoryBarrierCount = 1;
			depInfo.pImageMemoryBarriers = &imageBarrier;

			vkCmdPipelineBarrier2(commandBuffer.GetRaw(), &depInfo);
		}

		VkImageCreateInfo CreateImageInfo(
			VkFormat				format,
			VkExtent3D				extent,
			uint32_t				mipLevels,
			uint32_t				arrayLayers,
			VkSampleCountFlagBits	samples,
			VkImageTiling			tiling,
			VkImageUsageFlags		usageMask,
			VkImageLayout			initialLayout)
		{
			VkImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = format;
			imageCreateInfo.extent = extent;
			imageCreateInfo.mipLevels = mipLevels;
			imageCreateInfo.arrayLayers = arrayLayers;
			imageCreateInfo.samples = samples;
			imageCreateInfo.tiling = tiling;
			imageCreateInfo.usage = usageMask;
			imageCreateInfo.initialLayout = initialLayout;

			return imageCreateInfo;
		}

		VkImageViewCreateInfo CreateImageViewInfo(
			VkImage				image,
			VkFormat			format,
			VkImageAspectFlags	aspectMask)
		{
			VkImageViewCreateInfo imageViewInfo = {};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.pNext = nullptr;
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.image = image;
			imageViewInfo.format = format;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.layerCount = 1;
			imageViewInfo.subresourceRange.aspectMask = aspectMask;

			return imageViewInfo;
		}

		void BlitImageToImage(
			VulkanCommandBuffer& commandBuffer,
			VkImage				 source,
			VkImage				 destination,
			VkExtent3D			 srcSize,
			VkExtent3D			 dstSize)
		{
			VkImageBlit2 blitRegion = {};
			blitRegion.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2;
			blitRegion.pNext = nullptr;

			/* SOURCE */
			blitRegion.srcOffsets[1].x = srcSize.width;
			blitRegion.srcOffsets[1].y = srcSize.height;
			blitRegion.srcOffsets[1].z = 1;

			blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blitRegion.srcSubresource.baseArrayLayer = 0;
			blitRegion.srcSubresource.layerCount = 1;
			blitRegion.srcSubresource.mipLevel = 0;

			/* DST */
			blitRegion.dstOffsets[1].x = dstSize.width;
			blitRegion.dstOffsets[1].y = dstSize.height;
			blitRegion.dstOffsets[1].z = 1;

			blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blitRegion.dstSubresource.baseArrayLayer = 0;
			blitRegion.dstSubresource.layerCount = 1;
			blitRegion.dstSubresource.mipLevel = 0;

			/* INFO */
			VkBlitImageInfo2 blitInfo = {};
			blitInfo.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2;
			blitInfo.pNext = nullptr;
			blitInfo.dstImage = destination;
			blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			blitInfo.srcImage = source;
			blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			blitInfo.filter = VK_FILTER_LINEAR;
			blitInfo.regionCount = 1;
			blitInfo.pRegions = &blitRegion;

			vkCmdBlitImage2(commandBuffer.GetRaw(), &blitInfo);
		}

	}
}
