#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Core/LogSystem.h"
#include "Utils/KvazarUtils.h"


namespace Kvazar {

	/* SWAPCHAIN DATA */

	void SwapchainData::Flush()
	{
		nextImageIndex = 0;
		swapchain = VK_NULL_HANDLE;
		swapchainImagesFormat = VK_FORMAT_UNDEFINED;
		swapchainImagesExtent = { 0, 0, 0 };
		swapchainImages.clear();
		swapchainImageViews.clear();
	}

	void SwapchainData::Cleanup()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] Cleanup()...");

		VulkanContext* context = VulkanContext::GetContext();
		VkDevice logicalDevice = context->GetContextData().m_LogicalDevice.GetDevice();

		/* IMAGE VIEWS */
		for (VkImageView imageView : swapchainImageViews) 
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}
		swapchainImageViews.clear();

		/* RAW SWAPCHAIN */
		if (swapchain != VK_NULL_HANDLE) 
		{
			vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
			swapchain = VK_NULL_HANDLE;
		}
		else
		{
			KVAZAR_CRITICAL("[VulkanSwapchain] VkSwapchainKHR is VK_NULL_HANDLE");
		}

		/* OTHER */
		swapchainImages.clear();
		nextImageIndex = 0;
		swapchainImagesFormat = VK_FORMAT_UNDEFINED;
		swapchainImagesExtent = { 0, 0, 0 };
	}

	SwapchainData::SwapchainData()
	{
		Flush();
	}

	SwapchainData& SwapchainData::operator=(SwapchainData&& rData) noexcept
	{
		KVAZAR_DEBUG("[SwapchainData] operator=( SwapchainData&& rData )...");

		if (this != &rData)
		{
			nextImageIndex = rData.nextImageIndex;
			swapchain = rData.swapchain;
			swapchainImagesFormat = rData.swapchainImagesFormat;
			swapchainImagesExtent = rData.swapchainImagesExtent;
			swapchainImages = std::move(rData.swapchainImages);
			swapchainImageViews = std::move(rData.swapchainImageViews);

			rData.Flush();
		}

		return *this;
	}

	/* SWAPCHAIN */

	VulkanSwapchain::VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain()...");
	}

	VulkanSwapchain::VulkanSwapchain(SwapchainData&& rData) noexcept
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain( SwapchainData&& rData )...");

		m_Data = std::move(rData);
	}

	VulkanSwapchain& VulkanSwapchain::operator=(VulkanSwapchain&& rSwapchain) noexcept
	{
		KVAZAR_DEBUG("[VulkanSwapchain] operator=( VulkanSwapchain&& rSwapchain )...");

		if (this != &rSwapchain)
		{
			m_Data = std::move(rSwapchain.m_Data);
		}

		return *this;
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] ~VulkanSwapchain()...");
	}

	void VulkanSwapchain::Init()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] Init()...");
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
			m_Data.swapchain,
			UINT64_MAX,
			framesData.m_ImageAcquireSemaphore[currentFrameIndex],
			VK_NULL_HANDLE,
			&m_Data.nextImageIndex
		));
	}

	void VulkanSwapchain::EndFrame()
	{
		VulkanContext* context = VulkanContext::GetContext();
		FramesData& framesData = VulkanRendererAPI::GetFramesData();

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.pSwapchains = &m_Data.swapchain;
		presentInfo.swapchainCount = 1;
		presentInfo.pWaitSemaphores = &framesData.m_RenderFinishedSemaphore[m_Data.nextImageIndex];
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pImageIndices = &m_Data.nextImageIndex;
		presentInfo.pResults = nullptr;

		VK_CHECK(vkQueuePresentKHR(
			context->GetContextData().m_LogicalDevice.GetPresentationQueue(),
			&presentInfo
		));

		VulkanRendererAPI::SetNextFrameIndex();
	}

	void VulkanSwapchain::Shutdown()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] Shutdown()...");

		m_Data.Cleanup();
	}

	/* SWAPCHAIN BUILDER */

	VulkanSwapchainBuilder::VulkanSwapchainBuilder()
	{
		KVAZAR_DEBUG("[VulkanSwapchainBuilder] VulkanSwapchainBuilder()...");
	}

	VulkanSwapchainBuilder::~VulkanSwapchainBuilder()
	{
		KVAZAR_DEBUG("[VulkanSwapchainBuilder] ~VulkanSwapchainBuilder()...");
	}


	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetNextImageIndex(uint32_t nextImageIndex)
	{
		m_Data.nextImageIndex = nextImageIndex;
		return *this;
	}

	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetRawSwapchain(VkSwapchainKHR swapchain)
	{
		m_Data.swapchain = swapchain;
		return *this;
	}

	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImagesFormat(VkFormat swapchainImagesFormat)
	{
		m_Data.swapchainImagesFormat = swapchainImagesFormat;
		return *this;
	}

	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImagesExtent(VkExtent3D swapchainImagesExtent)
	{
		m_Data.swapchainImagesExtent = swapchainImagesExtent;
		return *this;
	}

	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImages(std::vector<VkImage>&& swapchainImages)
	{
		m_Data.swapchainImages = std::move(swapchainImages);
		return *this;
	}

	Kvazar::VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImageViews(std::vector<VkImageView>&& swapchainImageViews)
	{
		m_Data.swapchainImageViews = std::move(swapchainImageViews);
		return *this;
	}

	Kvazar::VulkanSwapchain VulkanSwapchainBuilder::Build()
	{
		return VulkanSwapchain(std::move(m_Data));
	}

}