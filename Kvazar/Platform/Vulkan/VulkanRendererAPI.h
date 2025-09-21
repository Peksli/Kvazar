#pragma once

#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

#include <cstdint>
#include <memory>
#include <array>


namespace Kvazar {

	constexpr uint8_t FRAMES_IN_FLIGHT = 2;

	struct FrameData
	{
		VkSemaphore imageAcquireSemaphore, renderFinishedSemahore;
		VkFence		workDoneFence;

		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
	};

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();
		virtual ~VulkanRendererAPI();

		virtual void Init()					override;
		virtual void Shutdown()				override;
		virtual void BeginFrame()			override;
		virtual void EndFrame()				override;
		virtual void BeginCommandBuffer()	override;
		virtual void EndCommandBuffer()		override;
		virtual void ExecuteCommandBuffer() override;
		virtual void ClearImage()			override;

		static FrameData& GetCurrentFrameData() { return m_FramesData[m_CurrentFrameIndex]; }

	private:
		//std::shared_ptr<VulkanSwapchain> m_Swapchain;

		static std::array<FrameData, FRAMES_IN_FLIGHT> m_FramesData;
		static uint8_t m_CurrentFrameIndex;
	};

}