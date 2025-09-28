#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"
#include "Utils/KvazarUtils.h"
#include "Platform/Vulkan/VMA.h"


namespace Kvazar {

	// Swapchain data

	VulkanSwapchainData::VulkanSwapchainData()
	{
		KVAZAR_DEBUG("[VulkanSwapchainData] VulkanSwapchainData() constructor called!!!");
	}

	VulkanSwapchainData::VulkanSwapchainData(VulkanSwapchainData&& rData)
	{
		KVAZAR_DEBUG("[VulkanSwapchainData] VulkanSwapchainData(VulkanSwapchainData&& rData) constructor called!!!");

		m_NextImageIndex = rData.m_NextImageIndex;
		m_Swapchain = rData.m_Swapchain;
		m_ImagesFormat = rData.m_ImagesFormat;
		m_ImagesExtent2D = rData.m_ImagesExtent2D;
		m_ImagesExtent3D = rData.m_ImagesExtent3D;

		m_Images = std::move(rData.m_Images);
		m_ImageViews = std::move(rData.m_ImageViews);
		m_OffscreenImages = std::move(rData.m_OffscreenImages);
		m_Allocator = std::move(rData.m_Allocator);
	}

	VulkanSwapchainData& VulkanSwapchainData::operator=(VulkanSwapchainData&& rData)
	{
		KVAZAR_DEBUG("[VulkanSwapchainData] operator=(VulkanSwapchainData&& rData) called!!!");

		if (this != &rData)
		{
			m_NextImageIndex = rData.m_NextImageIndex;
			m_Swapchain = rData.m_Swapchain;
			m_ImagesFormat = rData.m_ImagesFormat;
			m_ImagesExtent2D = rData.m_ImagesExtent2D;
			m_ImagesExtent3D = rData.m_ImagesExtent3D;

			m_Images = std::move(rData.m_Images);
			m_ImageViews = std::move(rData.m_ImageViews);
			m_OffscreenImages = std::move(rData.m_OffscreenImages);
			m_Allocator = std::move(rData.m_Allocator);
		}

		return *this;
	}

	VulkanSwapchainData::~VulkanSwapchainData()
	{
		KVAZAR_DEBUG("[VulkanSwapchainData] ~VulkanSwapchainData() called!!!");
	}

	void VulkanSwapchainData::Reset()
	{
		m_Swapchain = VK_NULL_HANDLE;
		m_Images.clear();
		m_ImageViews.clear();
		m_OffscreenImages.clear();
	}


	// Vulkan swapchain

	VulkanSwapchain::VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain() constructor called!!!");
	}

	VulkanSwapchain::VulkanSwapchain(VulkanSwapchainData&& rData)
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain(VulkanSwapchainData&& rData) constructor called!!!");

		m_SwapchainData = std::move(rData);
	}

	VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& rSwapchain) noexcept
	{
		KVAZAR_DEBUG("[VulkanSwapchain] VulkanSwapchain(VulkanSwapchain&& rSwapchain) constructor called!!!");

		m_SwapchainData = std::move(rSwapchain.m_SwapchainData);
		rSwapchain.m_SwapchainData.Reset();
	}

	VulkanSwapchain& VulkanSwapchain::operator=(VulkanSwapchain&& rSwapchain) noexcept
	{
		KVAZAR_DEBUG("[VulkanSwapchain] operator=(VulkanSwapchain&& rSwapchain) constructor called!!!");

		if (this != &rSwapchain)
		{
			m_SwapchainData = std::move(rSwapchain.m_SwapchainData);
			rSwapchain.m_SwapchainData.Reset();
		}

		return *this;
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] ~VulkanSwapchain() destructor called!!!");
	}	

	void VulkanSwapchain::Init()
	{
		KVAZAR_DEBUG("[VulkanSwapchain] Initializing swapchain!!!");
		m_SwapchainData.m_Allocator.Init();
		m_SwapchainData.m_OffscreenImages.resize(FRAMES_IN_FLIGHT);

		VulkanContext* context = VulkanContext::GetContext();

		int imageWidth, imageHeight;
		glfwGetFramebufferSize(context->GetContextData().m_Window, &imageWidth, &imageHeight);

		for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			// Extent + format
			VkExtent3D drawImageExtent =
			{
				static_cast<uint32_t>(imageWidth),
				static_cast<uint32_t>(imageHeight),
				1
			};
			m_SwapchainData.m_OffscreenImages[i].m_Extent		= drawImageExtent;
			m_SwapchainData.m_OffscreenImages[i].m_ImageFormat	= VK_FORMAT_R16G16B16A16_SFLOAT;

			// Usages
			VkImageUsageFlags usageMask = {};
			usageMask |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			usageMask |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			usageMask |= VK_IMAGE_USAGE_STORAGE_BIT;
			usageMask |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			// Extent

			VkImageCreateInfo imageCreateInfo = Utils::CreateImageInfo(
				m_SwapchainData.m_OffscreenImages[i].m_ImageFormat,
				m_SwapchainData.m_OffscreenImages[i].m_Extent,
				1,
				1,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				usageMask
			);

			// Want to use device local mem
			VmaAllocationCreateInfo allocCreateInfo = {};
			allocCreateInfo.usage			= VMA_MEMORY_USAGE_GPU_ONLY;
			allocCreateInfo.requiredFlags	= VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			vmaCreateImage(
				m_SwapchainData.m_Allocator.GetRaw(), 
				&imageCreateInfo,
				&allocCreateInfo,
				&m_SwapchainData.m_OffscreenImages[i].m_Image,
				&m_SwapchainData.m_OffscreenImages[i].m_Allocation,
				nullptr
			);

			// Image views
			VkImageViewCreateInfo imageViewCreateInfo = Utils::CreateImageViewInfo(
				m_SwapchainData.m_OffscreenImages[i].m_ImageFormat,
				m_SwapchainData.m_OffscreenImages[i].m_Image,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			VK_CHECK(vkCreateImageView(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				&imageViewCreateInfo,
				nullptr,
				&m_SwapchainData.m_OffscreenImages[i].m_ImageView
			));

		}
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

	void VulkanSwapchain::Shutdown()
	{
		VulkanContext* context = VulkanContext::GetContext();

		for (uint32_t i = 0; i < m_SwapchainData.m_Images.size(); i++)
		{
			vkDestroyImageView(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				m_SwapchainData.m_OffscreenImages[i].m_ImageView,
				nullptr
			);

			vmaDestroyImage(
				m_SwapchainData.m_Allocator.GetRaw(),
				m_SwapchainData.m_OffscreenImages[i].m_Image,
				m_SwapchainData.m_OffscreenImages[i].m_Allocation
			);

			vkDestroyImageView(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				m_SwapchainData.m_ImageViews[i],
				nullptr
			);
		}

		m_SwapchainData.m_Allocator.Shutdown();

		vkDestroySwapchainKHR(
			context->GetContextData().m_LogicalDevice.GetDevice(),
			m_SwapchainData.m_Swapchain,
			nullptr
		);
	}


	// BUILDER

	VulkanSwapchainBuilder::VulkanSwapchainBuilder()
	{

	}

	VulkanSwapchainBuilder::~VulkanSwapchainBuilder()
	{

	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetRaw(VkSwapchainKHR swapchain)
	{
		m_Data.m_Swapchain = swapchain;
		return *this;
	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetFormat(VkFormat format)
	{
		m_Data.m_ImagesFormat = format;
		return *this;
	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetExtent2D(const VkExtent2D& extent)
	{
		m_Data.m_ImagesExtent2D = extent;
		return *this;
	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetExtent3D(const VkExtent3D& extent3D)
	{
		m_Data.m_ImagesExtent3D = extent3D;
		return *this;
	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImages(std::vector<VkImage> images)
	{
		m_Data.m_Images = std::move(images);
		return *this;
	}

	VulkanSwapchainBuilder& VulkanSwapchainBuilder::SetImageViews(std::vector<VkImageView> views)
	{
		m_Data.m_ImageViews = std::move(views);
		return *this;
	}

	VulkanSwapchain VulkanSwapchainBuilder::Build()
	{
		return VulkanSwapchain(std::move(m_Data));
	}

}