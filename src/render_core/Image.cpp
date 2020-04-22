#pragma once
#include "Image.h"
#include <stdexcept>
#include "Application.h"
namespace Engine::RenderCore {
	Image::~Image()
	{
		vkDestroyImage(this->app->device.get(), this->get(), nullptr);
		vkFreeMemory(this->app->device.get(), imageMemory, nullptr);
	}

	void Image::init(Application* app)
	{
		this->app = app;
	}

	void Image::createImage(const VkImageCreateInfo& createInfo, const VkMemoryPropertyFlags& properties) {
		if (vkCreateImage(this->app->device.get(), &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create Image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(this->app->device.get(), this->get(), &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findImageMemoryType(memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(this->app->device.get(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(this->app->device.get(), this->get(), imageMemory, 0);
	}

	//void Image::updateImageMemory(unsigned char* imageData, uint32_t size)
	//{
	//	void* data;
	//	vkMapMemory(this->app->device.get(), imageMemory, 0, size, 0, &data);
	//	memcpy(data, imageData, static_cast<size_t>(size));
	//	vkUnmapMemory(this->app->device.get(), imageMemory);
	//}
	uint32_t Image::findImageMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(this->app->physicalDevice.get(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void Image::transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = this->app->commandPool.beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = this->get();
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		this->app->commandPool.endSingleTimeCommands(commandBuffer);
	}

}