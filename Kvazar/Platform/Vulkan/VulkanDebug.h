#pragma once

#include <vulkan/vulkan.h>


namespace Kvazar {

	class VulkanDebugger
	{
	public:
		VulkanDebugger();
		virtual ~VulkanDebugger();

		VkResult Create();
		void Shutdown();

		static VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerCreateInfo();

		VkDebugUtilsMessengerEXT GetRaw() const { return m_DebugMessenger; }

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
	};

}