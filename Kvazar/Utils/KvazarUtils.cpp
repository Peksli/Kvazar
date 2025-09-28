#include "Utils/KvazarUtils.h"


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
			VkImageLayout			newLayout)
		{
			VkImageMemoryBarrier2 imageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
			imageBarrier.pNext = nullptr;

			imageBarrier.srcStageMask	= srcStageMask; // first sync scope
			imageBarrier.srcAccessMask	= srcAccessMask;
			imageBarrier.dstStageMask	= dstStageMask; // second sync scope
			imageBarrier.dstAccessMask	= dstAccessMask;

			imageBarrier.oldLayout = oldLayout;
			imageBarrier.newLayout = newLayout;

			VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) 
				? VK_IMAGE_ASPECT_DEPTH_BIT 
				: VK_IMAGE_ASPECT_COLOR_BIT;

			VkImageSubresourceRange subRange = {};
			subRange.aspectMask		= aspectMask;
			subRange.baseArrayLayer = 0;
			subRange.layerCount		= 1;
			subRange.baseMipLevel	= 0;
			subRange.levelCount		= 1;

			imageBarrier.subresourceRange	= subRange;
			imageBarrier.image				= image;

			VkDependencyInfo depInfo{};
			depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
			depInfo.pNext = nullptr;

			depInfo.imageMemoryBarrierCount = 1;
			depInfo.pImageMemoryBarriers	= &imageBarrier;

			vkCmdPipelineBarrier2(commandBuffer.GetRaw(), &depInfo);
		}

		// Images and image views

		VkImageCreateInfo CreateImageInfo(
			VkFormat				format, 
			VkExtent3D				imageExtent,
			uint32_t				mipLevels,
			uint32_t				arrayLayers,
			VkSampleCountFlagBits	samples,
			VkImageTiling			tiling,
			VkImageUsageFlags		usageMask)
		{
			VkImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.pNext		= nullptr;
			imageCreateInfo.imageType	= VK_IMAGE_TYPE_2D;
			imageCreateInfo.format		= format;
			imageCreateInfo.extent		= imageExtent;

			imageCreateInfo.mipLevels	= mipLevels;
			imageCreateInfo.arrayLayers = arrayLayers;
			imageCreateInfo.samples		= samples;
			imageCreateInfo.tiling		= tiling;
			imageCreateInfo.usage		= usageMask;

			return imageCreateInfo;
		}

		VkImageViewCreateInfo CreateImageViewInfo(
			VkFormat				format, 
			VkImage					image, 
			VkImageAspectFlagBits	aspectMask)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType							= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext							= nullptr;
			imageViewCreateInfo.viewType						= VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.image							= image;
			imageViewCreateInfo.format							= format;
			imageViewCreateInfo.subresourceRange.aspectMask		= aspectMask;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.baseMipLevel	= 0;
			imageViewCreateInfo.subresourceRange.layerCount		= 1;
			imageViewCreateInfo.subresourceRange.levelCount		= 1;

			return imageViewCreateInfo;
		}

		void BlitImageToImage(
			VulkanCommandBuffer&	commandBuffer, 
			VkImage					srcImage, 
			VkImage					dstImage, 
			VkExtent3D				srcExtent, 
			VkExtent3D				dstExtent)
		{
			VkImageBlit2 blitRegion = {};
			blitRegion.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2;
			blitRegion.pNext = nullptr;

			blitRegion.srcOffsets[0] = { 0,0,0 };
			blitRegion.srcOffsets[1].x = srcExtent.width;
			blitRegion.srcOffsets[1].y = srcExtent.height;
			blitRegion.srcOffsets[1].z = 1;

			blitRegion.srcSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			blitRegion.srcSubresource.baseArrayLayer	= 0;
			blitRegion.srcSubresource.layerCount		= 1;
			blitRegion.srcSubresource.mipLevel			= 0;


			blitRegion.dstOffsets[0] = { 0,0,0 };
			blitRegion.dstOffsets[1].x = dstExtent.width;
			blitRegion.dstOffsets[1].y = dstExtent.height;
			blitRegion.dstOffsets[1].z = 1;

			blitRegion.dstSubresource.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			blitRegion.dstSubresource.baseArrayLayer	= 0;
			blitRegion.dstSubresource.layerCount		= 1;
			blitRegion.dstSubresource.mipLevel			= 0;


			VkBlitImageInfo2 imageBlitInfo = {};
			imageBlitInfo.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2;
			imageBlitInfo.pNext = nullptr;
			imageBlitInfo.srcImage			= srcImage;
			imageBlitInfo.srcImageLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			imageBlitInfo.dstImage			= dstImage;
			imageBlitInfo.dstImageLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageBlitInfo.filter			= VK_FILTER_LINEAR;
			imageBlitInfo.regionCount		= 1;
			imageBlitInfo.pRegions			= &blitRegion;

			vkCmdBlitImage2(commandBuffer.GetRaw(), &imageBlitInfo);
		}

	}
}