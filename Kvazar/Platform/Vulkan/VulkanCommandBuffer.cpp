//#include "Platform/Vulkan/VulkanCommandBuffer.h"
//#include "Platform/Vulkan/VulkanGraphicsContext.h"
//#include "Utils/KvazarUtils.h"
//#include "Core/LogSystem.h"
//
//
//namespace Kvazar {
//
//	// POOL 
//
//	void VulkanCommandPoolManager::Create(
//		VulkanCommandPool&			 commandPool, 
//		VkCommandPoolCreateFlagBits  bufferUsageMask, 
//		uint32_t					 queueFamilyIndex)
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		VkDevice logicalDevice = contextBuilder->GetVulkanLogicalDevice()->GetRaw();
//
//		VkCommandPoolCreateInfo poolInfo = {};
//		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//		poolInfo.pNext = nullptr;
//		poolInfo.flags = bufferUsageMask;
//		poolInfo.queueFamilyIndex = queueFamilyIndex;
//
//		VK_CHECK(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool.GetRaw()));
//	}
//
//	void VulkanCommandPoolManager::Destroy(
//		VulkanCommandPool& commandPool)
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		VkDevice logicalDevice = contextBuilder->GetVulkanLogicalDevice()->GetRaw();
//
//		vkDestroyCommandPool(logicalDevice, commandPool.GetRaw(), nullptr);
//	}
//
//	// CMD BUFFER 
//
//	void VulkanCommandBufferManager::Allocate(
//		VulkanCommandBuffer&	cmdBuffer,
//		VulkanCommandPool&		commandPool, 
//		CommandBufferLevel		level, 
//		uint32_t				commandBufferCount)
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		VkDevice logicalDevice = contextBuilder->GetVulkanLogicalDevice()->GetRaw();
//
//		cmdBuffer.SetState(CommandBufferState::Initial);
//
//		VkCommandBufferAllocateInfo allocInfo = {};
//		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//		allocInfo.pNext = nullptr;
//		allocInfo.commandPool = commandPool.GetRaw();
//		allocInfo.commandBufferCount = commandBufferCount;
//		
//		switch (level)
//		{
//		case CommandBufferLevel::Primary:		
//			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;		
//			cmdBuffer.SetLevel(CommandBufferLevel::Primary); 
//			break;
//
//		case CommandBufferLevel::Secondary:		
//			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;	
//			cmdBuffer.SetLevel(CommandBufferLevel::Secondary);
//			break;
//		}
//
//		VK_CHECK(vkAllocateCommandBuffers(logicalDevice, &allocInfo, &cmdBuffer.GetRaw()));
//	}
//
//	void VulkanCommandBufferManager::BeginRecording(
//		VulkanCommandBuffer&					commandBuffer,
//		VkCommandBufferUsageFlags				bufferUsageMask, 
//		const VkCommandBufferInheritanceInfo*	pInh)
//	{
//		commandBuffer.SetState(CommandBufferState::Recording);
//
//		VkCommandBufferBeginInfo beginInfo = {};
//		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//		beginInfo.pNext = nullptr;
//		beginInfo.flags = bufferUsageMask;
//		beginInfo.pInheritanceInfo = pInh;
//
//		VK_CHECK(vkBeginCommandBuffer(commandBuffer.GetRaw(), &beginInfo));
//	}
//
//	void VulkanCommandBufferManager::EndRecording(
//		VulkanCommandBuffer& commandBuffer)
//	{
//		vkEndCommandBuffer(commandBuffer.GetRaw());
//
//		commandBuffer.SetState(CommandBufferState::Executable);
//	}
//
//	void VulkanCommandBufferManager::Submit(
//		VulkanCommandBuffer&	cmdBuffer,
//		VkQueue					dstQueue, 
//		uint32_t				submitCount, 
//		const VkSubmitInfo2*	pSubmits, 
//		VkFence					fence)
//	{
//		cmdBuffer.SetState(CommandBufferState::Pending);
//
//		VK_CHECK(vkQueueSubmit2(dstQueue, submitCount, pSubmits, fence));
//	}
//
//	void VulkanCommandBufferManager::Reset(VulkanCommandBuffer& cmdBuffer)
//	{
//		VK_CHECK(vkResetCommandBuffer(cmdBuffer.GetRaw(), 0));
//	}
//
//}
