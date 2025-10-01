#define VMA_IMPLEMENTATION
#include "Platform/Vulkan/VMA.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Utils/KvazarUtils.h"


namespace Kvazar {

	VulkanVMA::VulkanVMA()
	{

	}

	VulkanVMA::~VulkanVMA()
	{

	}

	void VulkanVMA::Init()
	{
		VulkanContext* context = VulkanContext::GetContext();

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = context->GetContextData().m_PhysicalDevice;
		allocatorInfo.device = context->GetContextData().m_LogicalDevice.GetDevice();
		allocatorInfo.instance = context->GetContextData().m_Instance;
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		vmaCreateAllocator(&allocatorInfo, &m_Allocator);
	}

	void VulkanVMA::Shutdown()
	{
		vmaDestroyAllocator(m_Allocator);
	}

}