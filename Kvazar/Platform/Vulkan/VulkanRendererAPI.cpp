#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"

#include "Utils/KvazarUtils.h"


namespace Kvazar {

	Kvazar::FramesData VulkanRendererAPI::m_FramesData;

	uint8_t VulkanRendererAPI::m_CurrentFrameIndex;

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
		VulkanContext* context = VulkanContext::GetContext();
		m_CurrentFrameIndex = 0;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_FramesData.m_RenderFinishedSemaphore.resize(FRAMES_IN_FLIGHT);
		m_FramesData.m_ImageAcquireSemaphore.resize(FRAMES_IN_FLIGHT);
		m_FramesData.m_CommandBuffers.resize(FRAMES_IN_FLIGHT);
		m_FramesData.m_WorkDoneFences.resize(FRAMES_IN_FLIGHT);
		m_FramesData.m_CommandPools.resize(FRAMES_IN_FLIGHT);

		for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			vkCreateSemaphore(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				&semaphoreInfo,
				nullptr,
				&m_FramesData.m_ImageAcquireSemaphore[i]
			);

			vkCreateSemaphore(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				&semaphoreInfo,
				nullptr,
				&m_FramesData.m_RenderFinishedSemaphore[i]
			);

			vkCreateFence(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				&fenceInfo,
				nullptr,
				&m_FramesData.m_WorkDoneFences[i]
			);

			VulkanCommandPool::Create(
				m_FramesData.m_CommandPools[i],
				(VkCommandPoolCreateFlagBits)(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT),
				context->GetContextData().m_LogicalDevice.GetGraphicsIndex()
			);

			VulkanCommandBuffer::Allocate(
				m_FramesData.m_CommandBuffers[i],
				m_FramesData.m_CommandPools[i],
				CommandBufferLevel::Primary,
				1
			);
		}

		KVAZAR_DEBUG("VulkanRendererAPI initialized, ready to go...");
	}

	void VulkanRendererAPI::BeginFrame()
	{
		VulkanContext* context = VulkanContext::GetContext();
		context->GetContextData().m_Swapchain.BeginFrame();
	}

	void VulkanRendererAPI::BeginCommandBuffer()
	{
		VulkanCommandBuffer::Reset(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex]
		);

		VulkanCommandBuffer::BeginRecording(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex],
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			nullptr
		);
	}

	void VulkanRendererAPI::EndCommandBuffer()
	{
		VulkanCommandBuffer::EndRecording(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex]
		);
	}

	void VulkanRendererAPI::ExecuteCommandBuffer()
	{
		VulkanContext* context = VulkanContext::GetContext();
		VulkanSwapchain* swapchain = &context->GetContextData().m_Swapchain;
		VkQueue graphicsQueue = context->GetContextData().m_LogicalDevice.GetGraphicsQueue();

		VkSemaphoreSubmitInfo waitInfo = {};
		waitInfo.sType			= VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		waitInfo.pNext			= nullptr;
		waitInfo.semaphore		= m_FramesData.m_ImageAcquireSemaphore[m_CurrentFrameIndex];
		waitInfo.stageMask		= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT; // restrict 2nd sync scope
		waitInfo.deviceIndex	= 0;
		waitInfo.value			= 1;

		VkSemaphoreSubmitInfo signalInfo = {};
		signalInfo.sType		= VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		signalInfo.pNext		= nullptr;
		signalInfo.semaphore	= m_FramesData.m_RenderFinishedSemaphore[swapchain->GetNextImageIndex()];
		signalInfo.stageMask	= VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
		signalInfo.deviceIndex	= 0;
		signalInfo.value		= 1;

		VkCommandBufferSubmitInfo cmdSubmitInfo = {};
		cmdSubmitInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
		cmdSubmitInfo.pNext			= nullptr;
		cmdSubmitInfo.commandBuffer = m_FramesData.m_CommandBuffers[m_CurrentFrameIndex].GetRaw();
		cmdSubmitInfo.deviceMask	= 1;

		VkSubmitInfo2 submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.pNext = nullptr;
		submitInfo.flags = 0;
		submitInfo.waitSemaphoreInfoCount	= 1;
		submitInfo.pWaitSemaphoreInfos		= &waitInfo;
		submitInfo.commandBufferInfoCount	= 1;
		submitInfo.pCommandBufferInfos		= &cmdSubmitInfo;
		submitInfo.signalSemaphoreInfoCount = 1;
		submitInfo.pSignalSemaphoreInfos	= &signalInfo;

		EndCommandBuffer();

		VulkanCommandBuffer::Submit(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex],
			graphicsQueue,
			1,
			&submitInfo,
			m_FramesData.m_WorkDoneFences[m_CurrentFrameIndex]
		);
	}

	void VulkanRendererAPI::EndFrame()
	{
		VulkanContext* context = VulkanContext::GetContext();
		context->GetContextData().m_Swapchain.EndFrame();
	}

	void VulkanRendererAPI::Shutdown()
	{
		VulkanContext* context = VulkanContext::GetContext();
		vkDeviceWaitIdle(
			context->GetContextData().m_LogicalDevice.GetDevice()
		);

		for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			VulkanCommandPool::Destroy(
				m_FramesData.m_CommandPools[i]
			);

			vkDestroySemaphore(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				m_FramesData.m_ImageAcquireSemaphore[i],
				nullptr
			);

			vkDestroySemaphore(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				m_FramesData.m_RenderFinishedSemaphore[i],
				nullptr
			);

			vkDestroyFence(
				context->GetContextData().m_LogicalDevice.GetDevice(),
				m_FramesData.m_WorkDoneFences[i],
				nullptr
			);
		}
	}

	void VulkanRendererAPI::ClearImage()
	{
		VulkanContext* context = VulkanContext::GetContext();
		VulkanSwapchain* swapchain = &context->GetContextData().m_Swapchain;

		VkClearColorValue clearValue;
		float flashColor = std::abs(std::sin(glfwGetTime()));
		clearValue = { {0.0f, flashColor, 0.0f, 1.0f} };

		VkImageSubresourceRange range = {};
		range.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseArrayLayer	= 0;
		range.baseMipLevel		= 0;
		range.layerCount		= 1;
		range.levelCount		= 1;

		Utils::TransitionImageLayout(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex],
			swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_UNDEFINED, 
			VK_IMAGE_LAYOUT_GENERAL
		);

		vkCmdClearColorImage(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex].GetRaw(),
			swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_GENERAL,
			&clearValue,
			1,
			&range
		);

		Utils::TransitionImageLayout(
			m_FramesData.m_CommandBuffers[m_CurrentFrameIndex],
			swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_GENERAL, 
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		);
	}

}