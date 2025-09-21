//#pragma once
//
//#include "Renderer/SwapChain.h"
//
//#include <vector>
//
//
//namespace Kvazar {
//
//	struct SwapchainDetails
//	{
//		VkSurfaceCapabilitiesKHR		m_SurfaceCapabilities = {};
//		std::vector<VkSurfaceFormatKHR> m_SurfaceFormats;
//		std::vector<VkPresentModeKHR>	m_PresentModes;
//
//		bool IsComplete() 
//		{ 
//			return  !m_SurfaceFormats.empty()	&&
//					!m_PresentModes.empty()		&& 
//					 m_SurfaceCapabilities.minImageCount > 0; 
//		}
//		void Clear() { m_SurfaceCapabilities = {}; m_SurfaceFormats.clear(); m_PresentModes.clear(); }
//	};
//
//	class VulkanSwapchain : public Swapchain
//	{
//	public:
//		VulkanSwapchain();
//		virtual ~VulkanSwapchain();
//
//		virtual void Create()		override;
//		virtual void Shutdown()		override;
//
//		virtual void BeginFrame()	override;
//		virtual void EndFrame()		override;
//
//		VkSwapchainKHR GetRaw() const { return m_Swapchain; }
//		VkImage GetNextImage() { return m_Images[m_NextImageIndex]; }
//
//	private:
//		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
//		std::vector<VkImage> m_Images;
//		std::vector<VkImageView> m_ImageViews;
//
//		uint32_t m_NextImageIndex;
//	};
//
//	class VulkanSwapchainSelector
//	{
//	public:
//		VulkanSwapchainSelector();
//		virtual ~VulkanSwapchainSelector();
//
//		static VkSurfaceFormatKHR	ChooseSurfacaFormatFrom(std::vector<VkSurfaceFormatKHR>& formats);
//		static VkPresentModeKHR		ChoosePresentModeFrom(std::vector<VkPresentModeKHR>& presentModes);
//		static VkExtent2D			ChooseExtent2DFrom(VkSurfaceCapabilitiesKHR& capabilities);
//	};
//
//}

#pragma once

#include "Renderer/SwapChain.h"

#include <vector>


namespace Kvazar {

	struct VulkanSwapchainData
	{
		VkSwapchainKHR				m_Swapchain;
		VkFormat					m_ImagesFormat;
		VkExtent2D					m_ImagesExtent;
		std::vector<VkImage>		m_Images;
		std::vector<VkImageView>	m_ImageViews;
	};

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

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

	private:
		VulkanSwapchainData m_SwapchainData;
	};

}
