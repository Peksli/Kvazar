#include "Platform/Vulkan/VulkanImage.h"
#include "Core/LogSystem.h"
#include "Utils/KvazarUtils.h"


namespace Kvazar {

	void ImageData::Reset()
	{
		image = VK_NULL_HANDLE;
		view = VK_NULL_HANDLE;
		allocation = VK_NULL_HANDLE;
		format = VK_FORMAT_UNDEFINED;
		extent = { 0, 0, 0 };
		layout = VK_IMAGE_LAYOUT_UNDEFINED;
		usage = 0;
		memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
		aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		flags = 0;
	}

	/* IMAGE DATA */

	void ImageData::Cleanup(VkDevice device, VmaAllocator allocator)
	{
		if (view != VK_NULL_HANDLE) {
			vkDestroyImageView(device, view, nullptr);
			view = VK_NULL_HANDLE;
		}

		if (image != VK_NULL_HANDLE) {
			vmaDestroyImage(allocator, image, allocation);
			image = VK_NULL_HANDLE;
			allocation = VK_NULL_HANDLE;
		}

		format = VK_FORMAT_UNDEFINED;
		extent = { 0, 0, 0 };
		layout = VK_IMAGE_LAYOUT_UNDEFINED;
		usage = 0;
		memoryUsage = VMA_MEMORY_USAGE_UNKNOWN;
		aspect = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	/* IMAGE */

	VulkanImage::VulkanImage()
	{

	}

	VulkanImage::VulkanImage(ImageData&& data) noexcept
		: m_Data(std::move(data))
	{
		data.Reset();
	}

	VulkanImage::VulkanImage(VulkanImage&& rImage) noexcept
		: m_Data(std::move(rImage.m_Data))
	{
		rImage.m_Data.Reset();
	}

	Kvazar::VulkanImage& VulkanImage::operator=(VulkanImage&& rImage) noexcept
	{
		if (this != &rImage)
		{
			m_Data = std::move(rImage.m_Data);
			rImage.m_Data.Reset();
		}

		return *this;
	}

	void VulkanImage::Cleanup(VkDevice device, VmaAllocator allocator)
	{
		m_Data.Cleanup(device, allocator);
	}

	/* IMAGE BUILDER */

	VulkanImageBuilder::VulkanImageBuilder(VkDevice device, VmaAllocator allocator)
		: m_Device(device)
		, m_Allocator(allocator)
	{
	}

	VulkanImageBuilder& VulkanImageBuilder::SetFormat(VkFormat format)
	{
		m_Data.format = format;
		return *this;
	}

	VulkanImageBuilder& VulkanImageBuilder::SetExtent(VkExtent3D extent)
	{
		m_Data.extent = extent;
		return *this;
	}

	VulkanImageBuilder& VulkanImageBuilder::SetUsage(VkImageUsageFlags usage)
	{
		m_Data.usage = usage;
		return *this;
	}

	Kvazar::VulkanImageBuilder& VulkanImageBuilder::SetMemPropsFlags(VkMemoryPropertyFlags flags)
	{
		m_Data.flags = flags;
		return *this;
	}

	VulkanImageBuilder& VulkanImageBuilder::SetMemoryUsage(VmaMemoryUsage memoryUsage)
	{
		m_Data.memoryUsage = memoryUsage;
		return *this;
	}

	VulkanImageBuilder& VulkanImageBuilder::SetAspect(VkImageAspectFlags aspect)
	{
		m_Data.aspect = aspect;
		return *this;
	}

	VulkanImageBuilder& VulkanImageBuilder::SetLayout(VkImageLayout layout)
	{
		m_Data.layout = layout;
		return *this;
	}

	VulkanImage VulkanImageBuilder::Build()
	{
		/* IMAGE CREATION */
		VkImageCreateInfo imageInfo = Utils::CreateImageInfo(
			m_Data.format,
			m_Data.extent,
			1,
			1,
			VK_SAMPLE_COUNT_1_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			m_Data.usage,
			m_Data.layout
		);

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = m_Data.memoryUsage;
		allocInfo.requiredFlags = m_Data.flags;

		VK_CHECK(
			vmaCreateImage(
				m_Allocator, 
				&imageInfo, 
				&allocInfo, 
				&m_Data.image, 
				&m_Data.allocation, 
				nullptr)
		);

		/* IMAGE VIEW CREATION */
		VkImageViewCreateInfo viewInfo = Utils::CreateImageViewInfo(
			m_Data.image,
			m_Data.format,
			m_Data.aspect
		);

		VK_CHECK(vkCreateImageView(m_Device, &viewInfo, nullptr, &m_Data.view));

		return VulkanImage(std::move(m_Data));
	}

}