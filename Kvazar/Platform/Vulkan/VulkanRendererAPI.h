#pragma once

#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Renderer/RendererAPI.h"

#include <cstdint>
#include <memory>


namespace Kvazar {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();
		virtual ~VulkanRendererAPI();

		virtual void Init()	override;
		virtual void BeginFrame() override;
		virtual void BeginCommandBuffer() override;
		virtual void EndCommandBuffer()	override;
		virtual void ExecuteCommandBuffer() override;
		virtual void EndFrame()	override;
		virtual void Shutdown()	override;

	private:
		std::shared_ptr<VulkanSwapchain> m_Swapchain;
	};

}