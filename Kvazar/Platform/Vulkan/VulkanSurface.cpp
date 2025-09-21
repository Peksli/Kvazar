//#include "Platform/Vulkan/VulkanSurface.h"
//#include "Platform/Vulkan/VulkanGraphicsContext.h"
//#include "Core/LogSystem.h"
//
//
//namespace Kvazar {
//
//	VulkanSurface::VulkanSurface()
//	{
//		KVAZAR_DEBUG("[Vulkan Surface] VulkanSurface() called");
//	}
//
//	VulkanSurface::~VulkanSurface()
//	{
//		KVAZAR_DEBUG("[Vulkan Surface] ~VulkanSurface() called");
//	}
//
//	void VulkanSurface::Create()
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		if (glfwCreateWindowSurface(contextBuilder->GetInstance(), contextBuilder->GetContextSpecification().m_Window, nullptr, &m_Surface) != VK_SUCCESS)
//		{
//			KVAZAR_CRITICAL("Didn't create surface");
//		}
//	}
//
//	void VulkanSurface::Shutdown()
//	{
//		VulkanGraphicsContextBuilder* contextBuilder = VulkanGraphicsContextBuilder::GetContext();
//		vkDestroySurfaceKHR(contextBuilder->GetInstance(), m_Surface, nullptr);
//	}
//
//}