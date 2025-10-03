#include "Platform/Vulkan/VulkanLogicalDevice.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	/* DATA */

	void VulkanLogicalDeviceData::Reset()
	{
		m_Device = VK_NULL_HANDLE;
		m_GraphicsQueue = VK_NULL_HANDLE;
		m_PresentationQueue = VK_NULL_HANDLE;
		m_GraphicsQueueIndex = 0;
		m_PresentationQueueIndex = 0;
	}

	VulkanLogicalDeviceData::VulkanLogicalDeviceData()
	{
		Reset();
	}

	VulkanLogicalDeviceData& VulkanLogicalDeviceData::operator=(VulkanLogicalDeviceData&& rData) noexcept
	{
		KVAZAR_DEBUG("[VulkanLogicalDeviceData] operator=( VulkanLogicalDeviceData&& )...");

		if (this != &rData)
		{
			m_Device = rData.m_Device;
			m_GraphicsQueue = rData.m_GraphicsQueue;
			m_PresentationQueue = rData.m_PresentationQueue;
			m_GraphicsQueueIndex = rData.m_GraphicsQueueIndex;
			m_PresentationQueueIndex = rData.m_PresentationQueueIndex;

			rData.Reset();
		}

		return *this;
	}

	/* LOGICAL DEVICE */

	VulkanLogicalDevice::VulkanLogicalDevice()
	{
		KVAZAR_DEBUG("[VulkanLogicalDevice] VulkanLogicalDevice()...");
	}

	VulkanLogicalDevice::VulkanLogicalDevice(VulkanLogicalDeviceData&& rData) noexcept
	{
		KVAZAR_DEBUG("[VulkanLogicalDevice] VulkanLogicalDevice( VulkanLogicalDeviceData&& )...");

		m_Data = std::move(rData);
	}

	VulkanLogicalDevice& VulkanLogicalDevice::operator=(VulkanLogicalDevice&& rDevice) noexcept
	{
		KVAZAR_DEBUG("[VulkanLogicalDevice] operator=( VulkanLogicalDevice&& )...");

		if (this != &rDevice)
		{
			m_Data = std::move(rDevice.m_Data);
		}

		return *this;
	}

	VulkanLogicalDevice::~VulkanLogicalDevice()
	{
		KVAZAR_DEBUG("[VulkanLogicalDevice] ~VulkanLogicalDevice()...");
	}

	void VulkanLogicalDevice::Shutdown()
	{
		KVAZAR_DEBUG("[VulkanLogicalDevice] Shutdown...");

		vkDestroyDevice(m_Data.m_Device, nullptr);
	}

	/* LOGICAL DEVICE BUILDER */

	VulkanLogicalDeviceBuilder::VulkanLogicalDeviceBuilder()
	{
		KVAZAR_DEBUG("[VulkanLogicalDeviceBuilder] VulkanLogicalDeviceBuilder()...");
	}

	VulkanLogicalDeviceBuilder::~VulkanLogicalDeviceBuilder()
	{
		KVAZAR_DEBUG("[VulkanLogicalDeviceBuilder] ~VulkanLogicalDeviceBuilder()...");
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::Reset()
	{
		m_Data.Reset();
		return *this;
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::SetDevice(VkDevice device)
	{
		m_Data.m_Device = device;
		return *this;
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::SetGraphicsQueue(VkQueue graphicsQueue)
	{
		m_Data.m_GraphicsQueue = graphicsQueue;
		return *this;
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::SetPresentationQueue(VkQueue presentationQueue)
	{
		m_Data.m_PresentationQueue = presentationQueue;
		return *this;
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::SetPresentIndex(uint32_t presentIndex)
	{
		m_Data.m_PresentationQueueIndex = presentIndex;
		return *this;
	}

	VulkanLogicalDeviceBuilder& VulkanLogicalDeviceBuilder::SetGraphicsIndex(uint32_t graphicsIndex)
	{
		m_Data.m_GraphicsQueueIndex = graphicsIndex;
		return *this;
	}

	VulkanLogicalDevice VulkanLogicalDeviceBuilder::Build()
	{
		return VulkanLogicalDevice(std::move(m_Data));
	}

}