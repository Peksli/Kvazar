#pragma once

#include "Renderer/GraphicsContext.h"
#include "Platform/Vulkan/VulkanPhysicalDevice.h"
#include "Platform/Vulkan/VulkanLogicalDevice.h"
#include "Platform/Vulkan/VulkanSwapChain.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>


namespace Kvazar {

	using VkDebugMessenger = VkDebugUtilsMessengerEXT;

	struct VulkanContextData
	{
		GLFWwindow*				m_Window = nullptr;
		VkSurfaceKHR			m_Surface;

		VkInstance				m_Instance;
		VkDebugMessenger		m_DebugMessenger;
		VkPhysicalDevice		m_PhysicalDevice;
		VulkanLogicalDevice		m_LogicalDevice;
		VulkanSwapchain			m_Swapchain;
	};

	class VulkanContext : public Context
	{
	public:
		VulkanContext() = default;
		VulkanContext(const ContextSpec& spec);
		virtual ~VulkanContext();

		void Setup();
		void Shutdown();

		static	VulkanContext*	GetContext()		{ return m_Context;		}

		VulkanContextData&		GetContextData()	{ return m_ContextData; }

	private:
		static VulkanContext*	m_Context;
		VulkanContextData		m_ContextData;
	};

}