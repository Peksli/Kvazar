#pragma once

#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

#include <cstdint>
#include <memory>
#include <array>


namespace Kvazar {

	constexpr uint8_t FRAMES_IN_FLIGHT = 2; // internal linkage

	struct FramesData
	{
		std::vector<VkFence>		m_WorkDoneFences;
		std::vector<VkSemaphore>	m_ImageAcquireSemaphore;
		std::vector<VkSemaphore>	m_RenderFinishedSemaphore;

		std::vector<VulkanCommandBuffer>	m_CommandBuffers;
		std::vector<VulkanCommandPool>		m_CommandPools;
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

		static FramesData& GetFramesData() { return m_FramesData;			}
		static uint8_t GetNextFrameIndex() { return m_CurrentFrameIndex;	}
		static void SetNextFrameIndex() { m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % FRAMES_IN_FLIGHT; }

	private:
		static FramesData	m_FramesData;
		static uint8_t		m_CurrentFrameIndex;
	};

}