#pragma once

#include "Renderer/Surface.h"

#include <vulkan/vulkan.h>


namespace Kvazar {

	class VulkanSurface : public Surface
	{
	public:
		VulkanSurface();
		virtual ~VulkanSurface();

		void Create() override;
		void Shutdown() override;

		VkSurfaceKHR GetRaw() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
	};
	
}