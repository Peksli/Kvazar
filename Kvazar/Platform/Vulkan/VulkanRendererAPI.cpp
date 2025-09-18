#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"


namespace Kvazar {

	VulkanRendererAPI::VulkanRendererAPI()
	{
		KVAZAR_DEBUG("[VulkanRendererAPI] VulkanRendererAPI() called");
	}

	VulkanRendererAPI::~VulkanRendererAPI()
	{
		KVAZAR_DEBUG("[VulkanRendererAPI] ~VulkanRendererAPI() called");
	}

	void VulkanRendererAPI::Init()
	{
		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();

		m_Swapchain = contextBuilder->GetVulkanSwapchain();
	}

	void VulkanRendererAPI::BeginFrame()
	{
		m_Swapchain->BeginFrame();
	}

	void VulkanRendererAPI::BeginCommandBuffer()
	{

	}

	void VulkanRendererAPI::EndCommandBuffer()
	{

	}

	void VulkanRendererAPI::ExecuteCommandBuffer()
	{

	}

	void VulkanRendererAPI::EndFrame()
	{
		m_Swapchain->EndFrame();
	}

	void VulkanRendererAPI::Shutdown()
	{
		KVAZAR_DEBUG("[VulkanRendererAPI] Shutdown() called");
		m_Swapchain.reset();
	}

}