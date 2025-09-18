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

	private:
		VkCommandPool m_CommandPool;
	};

	class VulkanCommandPoolManager
	{
	public:
		VulkanCommandPoolManager() = default;
		virtual ~VulkanCommandPoolManager() = default;

		static void Create(
			VulkanCommandPool&			 commandPool,
			VkCommandPoolCreateFlagBits  bufferUsageMask,
			uint32_t					 queueFamilyIndex);

		static void Destroy(
			VulkanCommandPool& commandPool);
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

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer() = default;
		virtual ~VulkanCommandBuffer() = default;

		void SetState(CommandBufferState state) { m_State = state; }
		void SetLevel(CommandBufferLevel level) { m_Level = level; }

		VkCommandBuffer& GetRaw() { return m_CommandBuffer; }
		CommandBufferState GetState() const { return m_State; }
		CommandBufferLevel GetLevel() const { return m_Level; }

	private:
		VkCommandBuffer m_CommandBuffer;

		CommandBufferState m_State;
		CommandBufferLevel m_Level;
	};

	class VulkanCommandBufferManager
	{
	public:
		VulkanCommandBufferManager() = default;
		virtual ~VulkanCommandBufferManager() = default;

		static void Allocate(
			VulkanCommandBuffer&	cmdBuffer,
			VulkanCommandPool&		commandPool,
			CommandBufferLevel		level,
			uint32_t				commandBufferCount);

		static void BeginRecording(
			VulkanCommandBuffer&					commandBuffer,
			VkCommandBufferUsageFlags				bufferUsageMask, 
			const VkCommandBufferInheritanceInfo*	pInh);

		static void EndRecording(
			VulkanCommandBuffer& commandBuffer);

		static void Submit(
			VulkanCommandBuffer&	cmdBuffer,
			VkQueue					dstQueue,
			uint32_t				submitCount,
			const VkSubmitInfo2*	pSubmits,
			VkFence					fence);
	};

}