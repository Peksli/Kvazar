#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"
#include "Utils/KvazarUtils.h"


namespace Kvazar {

	// POOL

	void VulkanCommandPool::Create(VulkanCommandPool& commandPool, VkCommandPoolCreateFlagBits bufferUsageMask, 
		uint32_t queueFamilyIndex)
	{
		VulkanContext* context = VulkanContext::GetContext();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext				= nullptr;
		poolInfo.queueFamilyIndex	= queueFamilyIndex;
		poolInfo.flags				= bufferUsageMask;

		VK_CHECK(vkCreateCommandPool(
			context->GetContext()->GetContextData().m_LogicalDevice.GetDevice(),
			&poolInfo,
			nullptr,
			&commandPool.GetRaw()
		));
	}

	void VulkanCommandPool::Destroy(VulkanCommandPool& commandPool)
	{
		VulkanContext* context = VulkanContext::GetContext();

		vkDestroyCommandPool(
			context->GetContext()->GetContextData().m_LogicalDevice.GetDevice(),
			commandPool.GetRaw(),
			nullptr
		);
	}


	// BUFFER

	void VulkanCommandBuffer::Allocate(VulkanCommandBuffer& cmdBuffer, VulkanCommandPool& commandPool, 
		CommandBufferLevel level, uint32_t commandBufferCount)
	{
		VulkanContext* context = VulkanContext::GetContext();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandBufferCount = commandBufferCount;
		allocInfo.commandPool = commandPool.GetRaw();
		
		switch (level)
		{
		case CommandBufferLevel::Primary:	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;		break;
		case CommandBufferLevel::Secondary: allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;	break;
		default: KVAZAR_CRITICAL("Can not find appropriate level!!!");
		}

		vkAllocateCommandBuffers(
			context->GetContext()->GetContextData().m_LogicalDevice.GetDevice(),
			&allocInfo,
			&cmdBuffer.GetRaw()
		);
	}

	void VulkanCommandBuffer::BeginRecording(VulkanCommandBuffer& commandBuffer, VkCommandBufferUsageFlags bufferUsageMask, 
		const VkCommandBufferInheritanceInfo* pInh)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext				= nullptr;
		beginInfo.pInheritanceInfo	= pInh;
		beginInfo.flags				= bufferUsageMask;

		VK_CHECK(vkBeginCommandBuffer(
			commandBuffer.GetRaw(), 
			&beginInfo)
		);
	}

	void VulkanCommandBuffer::EndRecording(VulkanCommandBuffer& commandBuffer)
	{
		vkEndCommandBuffer(
			commandBuffer.GetRaw()
		);
	}

	void VulkanCommandBuffer::Submit(VkQueue dstQueue, uint32_t submitCount,
		const VkSubmitInfo2* pSubmits, VkFence fence)
	{
		vkQueueSubmit2(
			dstQueue,
			submitCount,
			pSubmits,
			fence
		);
	}

	void VulkanCommandBuffer::Reset(VulkanCommandBuffer& cmdBuffer)
	{
		vkResetCommandBuffer(
			cmdBuffer.GetRaw(),
			0
		);
	}

}
