#pragma once

#include "Platform/Vulkan/VulkanPhysicalDevice.h"

#include <vulkan/vulkan.h>
#include <memory>


namespace Kvazar {

	//class VulkanLogicalDevice {
	//public:
	//	VulkanLogicalDevice(VkDevice logicalDevice, VkQueue graphicsQueue, VkQueue presentationQueue);
	//	~VulkanLogicalDevice();

	//	VkDevice GetRaw() const { return m_LogicalDevice; }
	//	VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
	//	VkQueue GetPresentationQueue() const { return m_PresentationQueue; }

	//private:
	//	VkDevice m_LogicalDevice = VK_NULL_HANDLE;
	//	VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
	//	VkQueue m_PresentationQueue = VK_NULL_HANDLE;
	//};


	//class VulkanLogicalDeviceCreator
	//{
	//public:
	//	VulkanLogicalDeviceCreator();
	//	virtual ~VulkanLogicalDeviceCreator();

	//	static std::shared_ptr<VulkanLogicalDevice> CreateFrom(std::shared_ptr<VulkanPhysicalDevice> device);

	//private:
	//	static std::vector<const char*> requiredExtensions;
	//};
	
	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice() = default;
		virtual ~VulkanLogicalDevice() = default;

		void SetDevice(VkDevice device)							{ m_Device = device;				}
		void SetGraphicsQueue(VkQueue graphicsQueue)			{ m_GraphicsQueue = graphicsQueue;	}
		void SetPresentationQueue(VkQueue presentationQueue)	{ m_PresentationQueue;				}

		VkDevice GetDevice()			 { return m_Device;				}
		VkQueue  GetGraphicsQueue()		 { return m_GraphicsQueue;		}
		VkQueue  GetPresentationQueue()	 { return m_PresentationQueue;	}

	private:
		VkDevice m_Device;
		VkQueue  m_GraphicsQueue;
		VkQueue  m_PresentationQueue;
	};

}