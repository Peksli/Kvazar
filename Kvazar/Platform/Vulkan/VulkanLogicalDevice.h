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
	};
	
	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice();
		virtual ~VulkanLogicalDevice();

		void SetDevice(VkDevice device)							{ m_Data.m_Device = device;							}
		void SetGraphicsQueue(VkQueue graphicsQueue)			{ m_Data.m_GraphicsQueue = graphicsQueue;			}
		void SetPresentationQueue(VkQueue presentationQueue)	{ m_Data.m_PresentationQueue = presentationQueue;	}
		void SetPresentIndex(uint32_t presentIndex)				{ m_Data.m_PresentationQueueIndex = presentIndex;	}
		void SetGraphicsIndex(uint32_t graphicsIndex)			{ m_Data.m_GraphicsQueueIndex = graphicsIndex;		}

		VkDevice GetDevice()			const	{ return m_Data.m_Device;					}
		VkQueue  GetGraphicsQueue()		const	{ return m_Data.m_GraphicsQueue;			}
		VkQueue  GetPresentationQueue()	const	{ return m_Data.m_PresentationQueue;		}
		uint32_t GetGraphicsIndex()		const 	{ return m_Data.m_GraphicsQueueIndex;		}
		uint32_t GetPresentationIndex()	const 	{ return m_Data.m_PresentationQueueIndex;	}

	private:
		VulkanLogicalDeviceData m_Data;
	};

}