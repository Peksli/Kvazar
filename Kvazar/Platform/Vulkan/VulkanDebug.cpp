#include "Core/LogSystem.h"
#include "Platform/Vulkan/VulkanDebug.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFn(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	[[maybe_unused]]VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	[[maybe_unused]]void* pUserData)
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:	KVAZAR_TRACE(fmt::runtime("Diagnostic: {0}"), pCallbackData->pMessage);		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		KVAZAR_INFO(fmt::runtime("Info: {0}"), pCallbackData->pMessage);			break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	KVAZAR_WARN(fmt::runtime("Warning: {0}"), pCallbackData->pMessage);			break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		KVAZAR_ERROR(fmt::runtime("Error: {0}"), pCallbackData->pMessage);			break;
	default: KVAZAR_ERROR("Unknown message severity!");
	}

	return VK_FALSE; // no need to abort vulkan call that triggered validation layer
}


namespace Kvazar {

	VulkanDebugger::VulkanDebugger()
	{
		KVAZAR_DEBUG("[Vulkan Debugger] VulkanDebugger() called");
	}

	VulkanDebugger::~VulkanDebugger()
	{
		KVAZAR_DEBUG("[Vulkan Debugger] ~VulkanDebugger() called");
	}

	VkResult VulkanDebugger::Create()
	{
		VkInstance instance = VulkanGraphicsContextBuilder::GetContext()->GetInstance();
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = GetDebugMessengerCreateInfo();

		const char* fnName = "vkCreateDebugUtilsMessengerEXT";
		auto pfnCreate = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, fnName);
		if (!pfnCreate || instance == VK_NULL_HANDLE)
		{
			KVAZAR_CRITICAL(FMT_STRING("Didn't find {0} !!!"), fnName);
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		else
		{
			return pfnCreate(instance, &debugMessengerCreateInfo, nullptr, &m_DebugMessenger);
		}
	}

	void VulkanDebugger::Shutdown()
	{
		VkInstance instance = VulkanGraphicsContextBuilder::GetContext()->GetInstance();

		const char* fnName = "vkDestroyDebugUtilsMessengerEXT";
		auto pfnDestroy = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, fnName);
		if (!pfnDestroy || instance == VK_NULL_HANDLE)
		{
			KVAZAR_CRITICAL(FMT_STRING("Didn't find {0} !!!"), fnName);
			return;
		}
		else
		{
			pfnDestroy(instance, m_DebugMessenger, nullptr);
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT VulkanDebugger::GetDebugMessengerCreateInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
		debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerCreateInfo.pfnUserCallback = debugCallbackFn;
		debugMessengerCreateInfo.pUserData = nullptr;

		return debugMessengerCreateInfo;
	}

}