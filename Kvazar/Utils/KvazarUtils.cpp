#include "Utils/KvazarUtils.h"


namespace Kvazar {

	namespace Utils {

		/*
		Actually we can have srcStage(producer) - bottom of pipe
		and dstStage(consumer) - top of pipe <=> sync scopes

		But in such a case we will wait till last frame in flight in finished, which is not good
		because GPU is idling
		*/

		void TransitionImageLayout(
			VulkanCommandBuffer&	commandBuffer, 
			VkImage					image, 
			VkImageLayout			oldLayout, 
			VkImageLayout			newLayout)
		{
			VkImageMemoryBarrier2 imageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
			imageBarrier.pNext = nullptr;

			imageBarrier.srcStageMask	= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT; // first sync scope
			imageBarrier.srcAccessMask	= VK_ACCESS_2_MEMORY_WRITE_BIT;
			imageBarrier.dstStageMask	= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT; // second sync scope
			imageBarrier.dstAccessMask	= VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT; 

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