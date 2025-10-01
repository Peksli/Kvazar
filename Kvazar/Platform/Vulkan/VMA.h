#pragma once

#include "vk_mem_alloc.h"


namespace Kvazar {

	class VulkanVMA
	{
	public:
		VulkanVMA();
		virtual ~VulkanVMA();

		VmaAllocator& GetAllocator() { return m_Allocator; }

		void Init();
		void Shutdown();

	private:
		VmaAllocator m_Allocator;
	};

}