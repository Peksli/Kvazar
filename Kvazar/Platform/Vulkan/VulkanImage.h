#pragma once

#include "Platform/Vulkan/VMA.h"
#include <vulkan/vulkan.h>


namespace Kvazar {

	struct AllocatedImage
	{
		VkImage			m_Image;
		VkImageView		m_ImageView;
		VmaAllocation	m_Allocation;
		VkExtent3D		m_Extent;
		VkFormat		m_ImageFormat;
	};

	class VulkanImage
	{
	public:
		VulkanImage();
		virtual ~VulkanImage();

	private:
		AllocatedImage m_ImageData;
	};

}