#pragma once

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace Kvazar {

	struct ImageData
	{
		VkImage         image = VK_NULL_HANDLE;
		VkImageView     view = VK_NULL_HANDLE;
		VmaAllocation   allocation = VK_NULL_HANDLE;
		VkFormat        format = VK_FORMAT_UNDEFINED;
		VkExtent3D      extent = { 0, 0, 0 };
		VkImageLayout   layout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageUsageFlags usage = 0;
		VmaMemoryUsage  memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
		VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		VkMemoryPropertyFlags flags = 0;

		void Reset();
		void Cleanup(VkDevice device, VmaAllocator allocator);
		bool IsValid() const { return image != VK_NULL_HANDLE; }
	};

	class VulkanImage
	{
	public:
		VulkanImage();
		VulkanImage(ImageData&& data) noexcept;
		VulkanImage(VulkanImage&& rImage) noexcept;
		VulkanImage& operator=(VulkanImage&& rImage)noexcept;
		~VulkanImage() = default;

		const ImageData& GetImageData() const { return m_Data; }
		ImageData& GetImageData() { return m_Data; }

		VkImage         GetImage()      const { return m_Data.image; }
		VkImageView     GetView()       const { return m_Data.view; }
		VmaAllocation   GetAllocation() const { return m_Data.allocation; }
		VkFormat        GetFormat()     const { return m_Data.format; }
		VkExtent3D      GetExtent()     const { return m_Data.extent; }
		VkImageLayout   GetLayout()     const { return m_Data.layout; }

		void SetLayout(VkImageLayout layout) { m_Data.layout = layout; }
		void Cleanup(VkDevice device, VmaAllocator allocator);

		bool IsValid() const { return m_Data.IsValid(); }

	private:
		ImageData m_Data;
	};

	class VulkanImageBuilder
	{
	public:
		VulkanImageBuilder(VkDevice device, VmaAllocator allocator);
		~VulkanImageBuilder() = default;

		VulkanImageBuilder& SetFormat(VkFormat format);
		VulkanImageBuilder& SetExtent(VkExtent3D extent);
		VulkanImageBuilder& SetUsage(VkImageUsageFlags usage);
		VulkanImageBuilder& SetMemPropsFlags(VkMemoryPropertyFlags flags);
		VulkanImageBuilder& SetMemoryUsage(VmaMemoryUsage memoryUsage);
		VulkanImageBuilder& SetAspect(VkImageAspectFlags aspect);
		VulkanImageBuilder& SetLayout(VkImageLayout layout);

		VulkanImage Build();

	private:
		VkDevice      m_Device;
		VmaAllocator  m_Allocator;
		ImageData     m_Data;
	};

}