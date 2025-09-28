#pragma once

#include "vk_mem_alloc.h"


namespace Kvazar {

	class VulkanMemAllocator
	{
	public:
		VulkanMemAllocator();
		virtual ~VulkanMemAllocator();

		VulkanMemAllocator(const VulkanMemAllocator&) = delete;
		VulkanMemAllocator& operator=(const VulkanMemAllocator&) = delete;
		VulkanMemAllocator(VulkanMemAllocator&& rVMA) noexcept;
		VulkanMemAllocator& operator=(VulkanMemAllocator&& rVMA) noexcept;

		void Init();
		void Shutdown();

		VmaAllocator GetRaw() const { return m_Allocator; }

	private:
		VmaAllocator m_Allocator;
	};

}