#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"
#include "Utils/KvazarUtils.h"


namespace Kvazar {

	VulkanSwapchain::VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain() constructor called!!!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] ~VulkanSwapchain() destructor called!!!");
	}

	void VulkanSwapchain::BeginFrame()
	{
		VulkanContext* context = VulkanContext::GetContext();
		FramesData& framesData = VulkanRendererAPI::GetFramesData();
		uint8_t currentFrameIndex = VulkanRendererAPI::GetNextFrameIndex();

		VK_CHECK(vkWaitForFences(
			context->GetContextData().m_LogicalDevice.GetDevice(),
			1,
			&framesData.m_WorkDoneFences[currentFrameIndex],
			VK_TRUE,
			UINT64_MAX
		));

		VK_CHECK(vkResetFences(
			context->GetContextData().m_LogicalDevice.GetDevice(),
			1,
			&framesData.m_WorkDoneFences[currentFrameIndex]
		));

		VK_CHECK(vkAcquireNextImageKHR(
			context->GetContextData().m_LogicalDevice.GetDevice(),
			m_SwapchainData.m_Swapchain,
			UINT64_MAX,
			framesData.m_ImageAcquireSemaphore[currentFrameIndex],
			VK_NULL_HANDLE,
			&m_SwapchainData.m_NextImageIndex
		));
	}

	void VulkanSwapchain::EndFrame()
	{
		VulkanContext* context = VulkanContext::GetContext();
		FramesData& framesData = VulkanRendererAPI::GetFramesData();
		uint8_t currentFrameIndex = VulkanRendererAPI::GetNextFrameIndex();

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext				= nullptr;
		presentInfo.pSwapchains			= &m_SwapchainData.m_Swapchain;
		presentInfo.swapchainCount		= 1;
		presentInfo.pWaitSemaphores		= &framesData.m_RenderFinishedSemaphore[m_SwapchainData.m_NextImageIndex];
		presentInfo.waitSemaphoreCount	= 1;
		presentInfo.pImageIndices		= &m_SwapchainData.m_NextImageIndex;
		presentInfo.pResults			= nullptr;

		VK_CHECK(vkQueuePresentKHR(
			context->GetContextData().m_LogicalDevice.GetPresentationQueue(),
			&presentInfo
		));

		VulkanRendererAPI::SetNextFrameIndex();
	}

}