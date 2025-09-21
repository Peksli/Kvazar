//#include "Platform/Vulkan/VulkanSwapChain.h"
//#include "Platform/Vulkan/VulkanGraphicsContext.h"
//#include "Platform/Vulkan/VulkanPhysicalDevice.h"
//#include "Platform/Vulkan/VulkanLogicalDevice.h"
//#include "Utils/KvazarUtils.h"
//#include "Core/LogSystem.h"
//
//#include "Platform/Vulkan/VulkanRendererAPI.h"
//
//
//namespace Kvazar {
//
//	VulkanSwapchain::VulkanSwapchain()
//	{
//		KVAZAR_DEBUG("[Vulkan Swapchain] VulkanSwapchain() called");
//	}
//
//	VulkanSwapchain::~VulkanSwapchain()
//	{
//		KVAZAR_DEBUG("[Vulkan Swapchain] ~VulkanSwapchain() called");
//	}
//
//	void VulkanSwapchain::Create()
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//
//		VkSurfaceKHR		surface			= contextBuilder->GetVulkanSurface()->GetRaw();
//		VkPhysicalDevice	device			= contextBuilder->GetVulkanPhysicalDevice()->GetRaw();
//		VkDevice 			logicalDevice	= contextBuilder->GetVulkanLogicalDevice()->GetRaw();
//
//		SwapchainDetails details = contextBuilder->GetVulkanPhysicalDevice()->GetSwapchainDetails();
//
//		VkSurfaceFormatKHR	surfaceFormat	= VulkanSwapchainSelector::ChooseSurfacaFormatFrom(details.m_SurfaceFormats);
//		VkPresentModeKHR	presentMode		= VulkanSwapchainSelector::ChoosePresentModeFrom(details.m_PresentModes);
//		VkExtent2D			realExtent2D	= VulkanSwapchainSelector::ChooseExtent2DFrom(details.m_SurfaceCapabilities);
//
//		VkSwapchainCreateInfoKHR swapchainInfo{};
//		swapchainInfo.sType				= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//		swapchainInfo.surface			= surface;
//		swapchainInfo.minImageCount		= details.m_SurfaceCapabilities.minImageCount;
//		swapchainInfo.imageFormat		= surfaceFormat.format;
//		swapchainInfo.imageColorSpace	= surfaceFormat.colorSpace;
//		swapchainInfo.imageExtent		= realExtent2D;
//		swapchainInfo.imageArrayLayers	= 1;
//		swapchainInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//
//		QueueFamilyIndices  indices = VulkanPhysicalDeviceSelector::GetFamilyIndices(device);
//		uint32_t		    queueFamilyIndices[] = { indices.graphicsQueueFamily.value(), indices.presentationQueueFamily.value() };
//		if (indices.graphicsQueueFamily != indices.presentationQueueFamily)
//		{
//			swapchainInfo.imageSharingMode		= VK_SHARING_MODE_CONCURRENT;
//			swapchainInfo.queueFamilyIndexCount = 2;
//			swapchainInfo.pQueueFamilyIndices	= queueFamilyIndices;
//		}
//		else
//		{
//			KVAZAR_CRITICAL("Kvazar works with 2 different queues(graphics + present), instead 1 was choose");
//		}
//
//		swapchainInfo.preTransform		= details.m_SurfaceCapabilities.currentTransform;
//		swapchainInfo.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//		swapchainInfo.presentMode		= presentMode;
//		swapchainInfo.clipped			= VK_TRUE;
//		swapchainInfo.oldSwapchain		= VK_NULL_HANDLE;
//
//		if (vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
//		{
//			KVAZAR_CRITICAL("Didn't create swapchain!");
//		}
//
//		uint32_t imageCount = 0;
//		vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &imageCount, nullptr);
//		m_Images.resize(imageCount);
//		vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &imageCount, m_Images.data());
//
//		m_ImageViews.resize(imageCount);
//		for (uint32_t i = 0; i < imageCount; i++)
//		{
//			VkImageViewCreateInfo imageViewInfo = {};
//			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//			imageViewInfo.image = m_Images[i];
//			imageViewInfo.format = swapchainInfo.imageFormat;
//			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//			imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//			imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//			imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//			imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//			imageViewInfo.subresourceRange.baseArrayLayer = 0;
//			imageViewInfo.subresourceRange.layerCount = 1;
//			imageViewInfo.subresourceRange.baseMipLevel = 0;
//			imageViewInfo.subresourceRange.levelCount = 1;
//
//			VK_CHECK(vkCreateImageView(logicalDevice, &imageViewInfo, nullptr, &m_ImageViews[i]));
//		}
//	}
//
//	void VulkanSwapchain::Shutdown()
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		VkDevice logicalDevice = contextBuilder->GetVulkanLogicalDevice()->GetRaw();
//
//		for (size_t i = 0; i < m_ImageViews.size(); i++)
//		{
//			vkDestroyImageView(logicalDevice, m_ImageViews[i], nullptr);
//		}
//
//		vkDestroySwapchainKHR(logicalDevice, m_Swapchain, nullptr);
//	}
//
//	void VulkanSwapchain::BeginFrame()
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//
//		VK_CHECK(vkWaitForFences(
//			contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
//			1,
//			&VulkanRendererAPI::GetCurrentFrameData().workDoneFence,
//			VK_TRUE, 
//			UINT32_MAX
//			));
//
//		VK_CHECK(vkResetFences(
//			contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
//			1,
//			&VulkanRendererAPI::GetCurrentFrameData().workDoneFence
//		));
//
//		VK_CHECK(vkAcquireNextImageKHR(
//			contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
//			m_Swapchain,
//			UINT32_MAX,
//			VulkanRendererAPI::GetCurrentFrameData().imageAcquireSemaphore,
//			VK_NULL_HANDLE,
//			&m_NextImageIndex
//			));
//
//		VulkanCommandBufferManager::Reset(
//			VulkanRendererAPI::GetCurrentFrameData().commandBuffer
//		);
//	}
//
//	void VulkanSwapchain::EndFrame()
//	{
//		VkPresentInfoKHR presentInfo	= {};
//		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//		presentInfo.pNext				= nullptr;
//		presentInfo.swapchainCount		= 1;
//		presentInfo.pSwapchains			= &m_Swapchain;
//		presentInfo.waitSemaphoreCount	= 1;
//		presentInfo.pWaitSemaphores		= &VulkanRendererAPI::GetCurrentFrameData().renderFinishedSemahore;
//		presentInfo.pImageIndices		= &m_NextImageIndex;
//
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		VK_CHECK(vkQueuePresentKHR(
//			contextBuilder->GetVulkanLogicalDevice()->GetPresentationQueue(), 
//			&presentInfo));
//	}
//
//	VulkanSwapchainSelector::VulkanSwapchainSelector()
//	{
//
//	}
//
//	VulkanSwapchainSelector::~VulkanSwapchainSelector()
//	{
//
//	}
//
//	VkSurfaceFormatKHR VulkanSwapchainSelector::ChooseSurfacaFormatFrom(std::vector<VkSurfaceFormatKHR>& formats)
//	{
//		for (const auto& format : formats)
//		{
//			if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
//			{
//				return format;
//			}
//		}
//
//		return formats[0];
//	}
//
//	VkPresentModeKHR VulkanSwapchainSelector::ChoosePresentModeFrom(std::vector<VkPresentModeKHR>& presentModes)
//	{
//		for (const auto& presentMode : presentModes)
//		{
//			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
//			{
//				return presentMode;
//			}
//		}
//
//		return VK_PRESENT_MODE_FIFO_KHR;
//	}
//
//	VkExtent2D VulkanSwapchainSelector::ChooseExtent2DFrom(VkSurfaceCapabilitiesKHR& capabilities)
//	{
//		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
//		{
//			return capabilities.currentExtent;
//		}
//		else
//		{
//			VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//			GLFWwindow* window = contextBuilder->GetGLFWWindow();
//
//			int width, height;
//			glfwGetFramebufferSize(window, &width, &height);
//
//			VkExtent2D realExtent2D = { (uint32_t)width, (uint32_t)height };
//			realExtent2D.width = std::clamp(realExtent2D.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
//			realExtent2D.height = std::clamp(realExtent2D.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
//
//			return realExtent2D;
//		}
//	}
//
//}


#include "Platform/Vulkan/VulkanSwapChain.h"


namespace Kvazar {

	VulkanSwapchain::VulkanSwapchain()
	{

	}

	VulkanSwapchain::~VulkanSwapchain()
	{

	}

}