#pragma once

#include "Renderer/SwapChain.h"

#include <vector>


namespace Kvazar {

	struct SwapchainDetails
	{
		VkSurfaceCapabilitiesKHR		m_SurfaceCapabilities = {};
		std::vector<VkSurfaceFormatKHR> m_SurfaceFormats;
		std::vector<VkPresentModeKHR>	m_PresentModes;

		bool IsComplete() 
		{ 
			return  !m_SurfaceFormats.empty()	&&
					!m_PresentModes.empty()		&& 
					 m_SurfaceCapabilities.minImageCount > 0; 
		}
		void Clear() { m_SurfaceCapabilities = {}; m_SurfaceFormats.clear(); m_PresentModes.clear(); }
	};

	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		virtual void Create()		override;
		virtual void Shutdown()		override;

		virtual void BeginFrame()	override;
		virtual void EndFrame()		override;

		VkSwapchainKHR GetRaw() const { return m_Swapchain; }

	private:
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
	};

	class VulkanSwapchainSelector
	{
	public:
		VulkanSwapchainSelector();
		virtual ~VulkanSwapchainSelector();

		static VkSurfaceFormatKHR	ChooseSurfacaFormatFrom(std::vector<VkSurfaceFormatKHR>& formats);
		static VkPresentModeKHR		ChoosePresentModeFrom(std::vector<VkPresentModeKHR>& presentModes);
		static VkExtent2D			ChooseExtent2DFrom(VkSurfaceCapabilitiesKHR& capabilities);
	};

}