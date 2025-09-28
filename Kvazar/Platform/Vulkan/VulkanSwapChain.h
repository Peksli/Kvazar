#pragma once

#include "Renderer/SwapChain.h"
#include "Platform/Vulkan/VulkanImage.h"


#include <vector>


namespace Kvazar {

	struct VulkanSwapchainData
	{
		VulkanSwapchainData();
		virtual ~VulkanSwapchainData();

		VulkanSwapchainData(VulkanSwapchainData&& rData);
		VulkanSwapchainData& operator=(VulkanSwapchainData&& rData);

		void Reset();

		uint32_t                    m_NextImageIndex	= 0;
		VkSwapchainKHR              m_Swapchain			= VK_NULL_HANDLE;
		VkFormat                    m_ImagesFormat		= VK_FORMAT_UNDEFINED;
		VkExtent2D                  m_ImagesExtent2D{};
		VkExtent3D                  m_ImagesExtent3D{};
		std::vector<VkImage>        m_Images;
		std::vector<VkImageView>    m_ImageViews;
		std::vector<AllocatedImage> m_OffscreenImages;

		VulkanMemAllocator          m_Allocator;
	};


	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

		VulkanSwapchain(VulkanSwapchainData&& rData);
		VulkanSwapchain(VulkanSwapchain&& rSwapchain) noexcept;
		VulkanSwapchain& operator=(VulkanSwapchain&& rSwapchain) noexcept;

		virtual void Init()			override;
		virtual void BeginFrame()	override;
		virtual void EndFrame()		override;
		virtual void Shutdown()		override;

		uint32_t	GetNextImageIndex() { return m_SwapchainData.m_NextImageIndex;								}
		VkImage		GetNextImage()		{ return m_SwapchainData.m_Images[m_SwapchainData.m_NextImageIndex];	}

		const VulkanSwapchainData& GetSwapchainData() const { return m_SwapchainData; }

	private:
		VulkanSwapchainData m_SwapchainData;
	};


	class VulkanSwapchainBuilder
	{
	public:
		VulkanSwapchainBuilder();
		virtual ~VulkanSwapchainBuilder();

		VulkanSwapchainBuilder& SetRaw(VkSwapchainKHR swapchain);
		VulkanSwapchainBuilder& SetFormat(VkFormat format);
		VulkanSwapchainBuilder& SetExtent2D(const VkExtent2D& extent);
		VulkanSwapchainBuilder& SetExtent3D(const VkExtent3D& extent3D);
		VulkanSwapchainBuilder& SetImages(std::vector<VkImage> images);
		VulkanSwapchainBuilder& SetImageViews(std::vector<VkImageView> views);

		VulkanSwapchain Build();

	private:
		VulkanSwapchainData m_Data;
	};

}
