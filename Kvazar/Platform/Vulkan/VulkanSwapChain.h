#pragma once

#include "Renderer/SwapChain.h"

#include <vector>


namespace Kvazar {

	struct SwapchainData
	{
		uint32_t nextImageIndex;
		VkSwapchainKHR swapchain;
		VkFormat swapchainImagesFormat;
		VkExtent3D swapchainImagesExtent;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;

		VkImage GetNextImage() const { return swapchainImages[nextImageIndex]; }

		void Flush();
		void Cleanup();

		SwapchainData();
		SwapchainData& operator=(SwapchainData&&) noexcept;
	};

	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		VulkanSwapchain(SwapchainData&&) noexcept;
		VulkanSwapchain& operator=(VulkanSwapchain&&) noexcept;
		virtual ~VulkanSwapchain();

		virtual void Init() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Shutdown() override;

		const SwapchainData& GetData() const { return m_Data; }

	private:
		SwapchainData m_Data;
	};

	class VulkanSwapchainBuilder
	{
	public:
		VulkanSwapchainBuilder();
		virtual ~VulkanSwapchainBuilder();

		VulkanSwapchainBuilder& SetNextImageIndex(uint32_t nextImageIndex);
		VulkanSwapchainBuilder& SetRawSwapchain(VkSwapchainKHR swapchain);
		VulkanSwapchainBuilder& SetImagesFormat(VkFormat swapchainImagesFormat);
		VulkanSwapchainBuilder& SetImagesExtent(VkExtent3D swapchainImagesExtent);
		VulkanSwapchainBuilder& SetImages(std::vector<VkImage>&& swapchainImages);
		VulkanSwapchainBuilder& SetImageViews(std::vector<VkImageView>&& swapchainImageViews);
		VulkanSwapchain Build();

	private:
		SwapchainData m_Data;
	};

}
