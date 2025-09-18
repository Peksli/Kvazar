#pragma once

#include "Renderer/GraphicsContext.h"
#include "Platform/Vulkan/VulkanDebug.h"
#include "Platform/Vulkan/VulkanPhysicalDevice.h"
#include "Platform/Vulkan/VulkanLogicalDevice.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanSurface.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>


namespace Kvazar {

	class VulkanGraphicsContextBuilder : public GraphicsContextBuilder
	{
	public:
		VulkanGraphicsContextBuilder(const GraphicsContextSpecification& specification);
		virtual ~VulkanGraphicsContextBuilder();

		void Build() override;
		void Destroy() override;

		static VulkanGraphicsContextBuilder* GetContext() { return s_VulkanContextBuilder; }
		const GraphicsContextSpecification& GetContextSpecification() const { return m_ContextSpecification; }
		VkInstance GetInstance() const { return instance; }
		std::shared_ptr<VulkanDebugger> GetVulkanDebugger() const { return m_VulkanDebugger; }
		std::shared_ptr<VulkanPhysicalDevice> GetVulkanPhysicalDevice() const { return m_PhysicalDevice; }
		std::shared_ptr<VulkanLogicalDevice> GetVulkanLogicalDevice() const { return m_LogicalDevice; }
		std::shared_ptr<VulkanSurface> GetVulkanSurface() const { return m_Surface; }
		std::shared_ptr<VulkanSwapchain> GetVulkanSwapchain() const { return m_Swapchain; }
		GLFWwindow* GetGLFWWindow() const { return m_ContextSpecification.m_Window; }

	private:
		void BuildInstance();			void DestroyInstance();
		void BuildDebugger();			void DestroyDebugger();
		void BuildPhysicalDevice();		void DestroyPhysicalDevice();
		void BuildLogicalDevice();		void DestroyLogicalDevice();
		void BuildSwapChain();			void DestroySwapChain();
		void BuildSurface();			void DestroySurface();

		static std::vector<const char*> GetExtensions();
		static std::vector<const char*> GetLayers();

	private:
		static VulkanGraphicsContextBuilder* s_VulkanContextBuilder;

		const GraphicsContextSpecification& m_ContextSpecification;

		VkInstance instance;
		std::shared_ptr<VulkanDebugger> m_VulkanDebugger;
		std::shared_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
		std::shared_ptr<VulkanLogicalDevice> m_LogicalDevice;
		std::shared_ptr<VulkanSwapchain> m_Swapchain;
		std::shared_ptr<VulkanSurface> m_Surface;
	};

}