#pragma once

#include "Renderer/SwapChain.h"

#include <vector>


namespace Kvazar {

	struct VulkanSwapchainData
	{
		uint32_t					m_NextImageIndex = 0;
		VkSwapchainKHR				m_Swapchain;
		VkFormat					m_ImagesFormat;
		VkExtent2D					m_ImagesExtent;
		std::vector<VkImage>		m_Images;
		std::vector<VkImageView>	m_ImageViews;
	};

	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		virtual void BeginFrame()	override;
		virtual void EndFrame()		override;

		void SetRaw(VkSwapchainKHR swapchain)						{ m_SwapchainData.m_Swapchain = swapchain;	}
		void SetFormat(VkFormat format)								{ m_SwapchainData.m_ImagesFormat = format;	}
		void SetExtent2D(const VkExtent2D& extent)					{ m_SwapchainData.m_ImagesExtent = extent;	}
		void SetImages(const std::vector<VkImage>& images)			{ m_SwapchainData.m_Images = images;		}
		void SetImageViews(const std::vector<VkImageView>& views)	{ m_SwapchainData.m_ImageViews = views;		}

		VkSwapchainKHR				GetRaw()		{ return m_SwapchainData.m_Swapchain;		}
		VkFormat					GetFormat()		{ return m_SwapchainData.m_ImagesFormat;	}
		VkExtent2D					GetExtent2D()	{ return m_SwapchainData.m_ImagesExtent;	}
		std::vector<VkImage>&		GetImages()		{ return m_SwapchainData.m_Images;			}
		std::vector<VkImageView>&	GetImageViews()	{ return m_SwapchainData.m_ImageViews;		}

		uint32_t GetNextImageIndex() { return m_SwapchainData.m_NextImageIndex; }
		VkImage GetNextImage() { return m_SwapchainData.m_Images[m_SwapchainData.m_NextImageIndex]; }

	private:
		VulkanSwapchainData m_SwapchainData;
	};

}
