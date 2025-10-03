#pragma once

#include <vulkan/vulkan.h>


namespace Kvazar {

	// POOL

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool() = default;
		virtual ~VulkanCommandPool() = default;

		VkCommandPool& GetRaw() { return m_CommandPool; }

		static void Create(VulkanCommandPool& commandPool, VkCommandPoolCreateFlagBits bufferUsageMask, 
			uint32_t queueFamilyIndex);

		static void Destroy(VulkanCommandPool& commandPool);

	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};


	// CMD BUFFER

	enum class CommandBufferState
	{
		Initial		= 0,
		Recording	= 1, 
		Executable	= 2,
		Pending		= 3,
		Invalid		= 4
	};

	enum class CommandBufferLevel
	{
		Primary		= 0,
		Secondary	= 1
	};

	struct VulkanCommandBufferData
	{
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;

		CommandBufferState m_State = CommandBufferState::Initial;
		CommandBufferLevel m_Level = CommandBufferLevel::Primary;
	};

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer() = default;
		virtual ~VulkanCommandBuffer() = default;

		void SetState(CommandBufferState state) { m_Data.m_State = state; }
		void SetLevel(CommandBufferLevel level) { m_Data.m_Level = level; }

		VkCommandBuffer&	GetRaw()			{ return m_Data.m_CommandBuffer;	}
		CommandBufferState	GetState() const	{ return m_Data.m_State;			}
		CommandBufferLevel	GetLevel() const	{ return m_Data.m_Level;			}

		static void Allocate(VulkanCommandBuffer& cmdBuffer, VulkanCommandPool& commandPool, CommandBufferLevel	level,
			uint32_t commandBufferCount);

		static void BeginRecording(VulkanCommandBuffer& commandBuffer, VkCommandBufferUsageFlags bufferUsageMask, 
			const VkCommandBufferInheritanceInfo* pInh);

		static void EndRecording(VulkanCommandBuffer& commandBuffer);

		static void Submit(VkQueue dstQueue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, 
			VkFence	fence);

		static void Reset(VulkanCommandBuffer& cmdBuffer);

	private:
		VulkanCommandBufferData m_Data;
	};

}