#include "Utils/KvazarUtils.h"


namespace Kvazar {

	namespace Utils {

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
			subRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
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

	}
}