#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Core/LogSystem.h"

#include "Utils/KvazarUtils.h"


namespace Kvazar {

	std::array<FrameData, FRAMES_IN_FLIGHT> VulkanRendererAPI::m_FramesData;

	uint8_t VulkanRendererAPI::m_CurrentFrameIndex = 0;

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
		//VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();

		//m_Swapchain = contextBuilder->GetVulkanSwapchain();

		//VkSemaphoreCreateInfo semaphoreInfo = {};
		//semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		//semaphoreInfo.pNext = nullptr;
		//semaphoreInfo.flags = 0;

		//VkFenceCreateInfo fenceInfo = {};
		//fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		//fenceInfo.pNext = nullptr;
		//fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		//for (uint8_t i = 0; i < FRAMES_IN_FLIGHT; i++)
		//{
		//	VK_CHECK(vkCreateSemaphore(
		//		contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
		//		&semaphoreInfo,
		//		nullptr,
		//		&m_FramesData[m_CurrentFrameIndex].imageAcquireSemaphore
		//	));

		//	VK_CHECK(vkCreateSemaphore(
		//		contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
		//		&semaphoreInfo,
		//		nullptr,
		//		&m_FramesData[m_CurrentFrameIndex].renderFinishedSemahore
		//	));

		//	VK_CHECK(vkCreateFence(
		//		contextBuilder->GetVulkanLogicalDevice()->GetRaw(),
		//		&fenceInfo,
		//		nullptr,
		//		&m_FramesData[m_CurrentFrameIndex].workDoneFence
		//	));

		//	QueueFamilyIndices indices = 
		//		VulkanPhysicalDeviceSelector::GetFamilyIndices(contextBuilder->GetVulkanPhysicalDevice()->GetRaw());

		//	VulkanCommandPoolManager::Create(
		//		m_FramesData[m_CurrentFrameIndex].commandPool,
		//		(VkCommandPoolCreateFlagBits)(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT),
		//		indices.graphicsQueueFamily.value()
		//	);

		//	VulkanCommandBufferManager::Allocate(
		//		m_FramesData[m_CurrentFrameIndex].commandBuffer,
		//		m_FramesData[m_CurrentFrameIndex].commandPool,
		//		CommandBufferLevel::Primary,
		//		1
		//	);
		//}
	}

	void VulkanRendererAPI::BeginFrame()
	{
		//m_Swapchain->BeginFrame();
	}

	void VulkanRendererAPI::BeginCommandBuffer()
	{
		//VulkanCommandBufferManager::BeginRecording(
		//	m_FramesData[m_CurrentFrameIndex].commandBuffer	,
		//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT		,
		//	nullptr);
	}

	void VulkanRendererAPI::EndCommandBuffer()
	{
		//VulkanCommandBufferManager::EndRecording(
		//	m_FramesData[m_CurrentFrameIndex].commandBuffer);
	}

	void VulkanRendererAPI::ExecuteCommandBuffer()
	{
		//EndCommandBuffer();

		//VkSemaphoreSubmitInfo signalInfo = {};
		//signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		//signalInfo.pNext = nullptr;
		//signalInfo.value = 1;
		//signalInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT; // restrain first sync scope
		//signalInfo.semaphore = m_FramesData[m_CurrentFrameIndex].renderFinishedSemahore;
		//signalInfo.deviceIndex = 0;

		//VkSemaphoreSubmitInfo waitInfo = {};
		//waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
		//waitInfo.pNext = nullptr;
		//waitInfo.value = 1;
		//waitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT; // restrain second sync scope
		//waitInfo.semaphore = m_FramesData[m_CurrentFrameIndex].imageAcquireSemaphore;
		//waitInfo.deviceIndex = 0;

		//VkCommandBufferSubmitInfo cmdSubmitInfo = {};
		//cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
		//cmdSubmitInfo.pNext = nullptr;
		//cmdSubmitInfo.commandBuffer = m_FramesData[m_CurrentFrameIndex].commandBuffer.GetRaw();
		//cmdSubmitInfo.deviceMask = 0;

		//VkSubmitInfo2 submitInfo = {};
		//submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		//submitInfo.pNext = nullptr;
		//submitInfo.flags = 0;
		//submitInfo.waitSemaphoreInfoCount = 1;
		//submitInfo.pWaitSemaphoreInfos = &waitInfo;
		//submitInfo.commandBufferInfoCount = 1;
		//submitInfo.pCommandBufferInfos = &cmdSubmitInfo;
		//submitInfo.signalSemaphoreInfoCount = 1;
		//submitInfo.pSignalSemaphoreInfos = &signalInfo;

		//VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
		//VK_CHECK(vkQueueSubmit2(
		//	contextBuilder->GetVulkanLogicalDevice()->GetGraphicsQueue(), 
		//	1, 
		//	&submitInfo, 
		//	m_FramesData[m_CurrentFrameIndex].workDoneFence
		//));
	}

	void VulkanRendererAPI::EndFrame()
	{
		//m_Swapchain->EndFrame();

		//m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % FRAMES_IN_FLIGHT;
	}

	void VulkanRendererAPI::Shutdown()
	{
		//KVAZAR_DEBUG("[VulkanRendererAPI] Shutdown() called");
		//m_Swapchain.reset();
	}

	void VulkanRendererAPI::ClearImage()
	{
		/*VkClearColorValue clearValue;
		float flashColor = std::abs(std::sin(glfwGetTime()));
		clearValue = { {0.0f, 0.0f, flashColor, 1.0f} };

		VkImageSubresourceRange subresRange = {};
		subresRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresRange.baseArrayLayer = 0;
		subresRange.layerCount = VK_REMAINING_MIP_LEVELS;
		subresRange.baseMipLevel = 0;
		subresRange.levelCount = VK_REMAINING_ARRAY_LAYERS;

		Utils::TransitionImageLayout(
			m_FramesData[m_CurrentFrameIndex].commandBuffer,
			m_Swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_GENERAL
			);

		vkCmdClearColorImage(
			m_FramesData[m_CurrentFrameIndex].commandBuffer.GetRaw(),
			m_Swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_GENERAL,
			&clearValue,
			1,
			&subresRange
		);

		Utils::TransitionImageLayout(
			m_FramesData[m_CurrentFrameIndex].commandBuffer,
			m_Swapchain->GetNextImage(),
			VK_IMAGE_LAYOUT_GENERAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		);*/
	}

}