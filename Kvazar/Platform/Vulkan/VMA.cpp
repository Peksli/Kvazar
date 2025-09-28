#define VMA_IMPLEMENTATION
#define VMA_DEBUG_DETECT_LEAKS 1
#include "vk_mem_alloc.h"

#include "Platform/Vulkan/VMA.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	VulkanMemAllocator::VulkanMemAllocator()
	{
		KVAZAR_DEBUG("[Vulkan Mem Allocator] VulkanMemAllocator() called!!!");
	}

	VulkanMemAllocator::VulkanMemAllocator(VulkanMemAllocator&& rVMA) noexcept
	{
		KVAZAR_DEBUG("[Vulkan Mem Allocator] VulkanMemAllocator(VulkanMemAllocator&& rVMA) called!!!");

		m_Allocator = rVMA.m_Allocator;
		rVMA.m_Allocator = VK_NULL_HANDLE;
	}

	VulkanMemAllocator& VulkanMemAllocator::operator=(VulkanMemAllocator&& rVMA) noexcept
	{
		KVAZAR_DEBUG("[Vulkan Mem Allocator] operator=(VulkanMemAllocator&& rVMA) called!!!");

		if (this != &rVMA)
		{
			m_Allocator = rVMA.m_Allocator;
			rVMA.m_Allocator = VK_NULL_HANDLE;
		}

		return *this;
	}

	VulkanMemAllocator::~VulkanMemAllocator()
	{
		KVAZAR_DEBUG("[Vulkan Mem Allocator] ~VulkanMemAllocator() called!!!");
	}

	void VulkanMemAllocator::Init()
	{
		VulkanContext* context = VulkanContext::GetContext();

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice	= context->GetContextData().m_PhysicalDevice;
		allocatorInfo.device			= context->GetContextData().m_LogicalDevice.GetDevice();
		allocatorInfo.instance			= context->GetContextData().m_Instance;
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

		vmaCreateAllocator(&allocatorInfo, &m_Allocator);
	}

	void VulkanMemAllocator::Shutdown()
	{
		vmaDestroyAllocator(m_Allocator);
	}

}