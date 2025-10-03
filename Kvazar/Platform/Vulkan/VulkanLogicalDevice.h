#pragma once

#include "Platform/Vulkan/VulkanPhysicalDevice.h"

#include <vulkan/vulkan.h>
#include <memory>


namespace Kvazar {

	struct VulkanLogicalDeviceData
	{
		VkDevice m_Device;
		VkQueue  m_GraphicsQueue;
		VkQueue  m_PresentationQueue;
		uint32_t m_GraphicsQueueIndex;
		uint32_t m_PresentationQueueIndex;

		void Reset();
		VulkanLogicalDeviceData();
		VulkanLogicalDeviceData& operator=(VulkanLogicalDeviceData&&) noexcept;
	};
	
	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice();
		VulkanLogicalDevice(VulkanLogicalDeviceData&&) noexcept;
		VulkanLogicalDevice& operator=(VulkanLogicalDevice&&) noexcept;
		virtual ~VulkanLogicalDevice();

		void Shutdown();

		VkDevice GetDevice()			const	{ return m_Data.m_Device;					}
		VkQueue  GetGraphicsQueue()		const	{ return m_Data.m_GraphicsQueue;			}
		VkQueue  GetPresentationQueue()	const	{ return m_Data.m_PresentationQueue;		}
		uint32_t GetGraphicsIndex()		const 	{ return m_Data.m_GraphicsQueueIndex;		}
		uint32_t GetPresentationIndex()	const 	{ return m_Data.m_PresentationQueueIndex;	}

	private:
		VulkanLogicalDeviceData m_Data;
	};

	class VulkanLogicalDeviceBuilder
	{
	public:
		VulkanLogicalDeviceBuilder();
		virtual ~VulkanLogicalDeviceBuilder();

		VulkanLogicalDeviceBuilder& Reset();
		VulkanLogicalDeviceBuilder& SetDevice(VkDevice device);
		VulkanLogicalDeviceBuilder& SetGraphicsQueue(VkQueue graphicsQueue);
		VulkanLogicalDeviceBuilder& SetPresentationQueue(VkQueue presentationQueue);
		VulkanLogicalDeviceBuilder& SetPresentIndex(uint32_t presentIndex);
		VulkanLogicalDeviceBuilder& SetGraphicsIndex(uint32_t graphicsIndex);
		VulkanLogicalDevice Build();

	private:
		VulkanLogicalDeviceData m_Data;
	};

}