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

		void SetDebugMessenger(VkDebugUtilsMessengerEXT messenger) { m_DebugMessenger = messenger; }
		VkDebugUtilsMessengerEXT GetRaw() { return m_DebugMessenger; }

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

	class VulkanDebugMessenger
	{
	public:
		VulkanDebugMessenger() = default;
		virtual ~VulkanDebugMessenger() = default;

		void SetMessenger(VkDebugUtilsMessengerEXT messenger) { m_DebugMessenger = messenger; }

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}