#pragma once
//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include "BufferFactory.h"
#include "Application.h"

namespace Engine::RenderCore::BufferFactory {
	std::unique_ptr<Buffer> createStagingBuffer(Application* app, VkDeviceSize size)
	{
		std::unique_ptr<Buffer> stagingBuffer(new Buffer());
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		stagingBuffer->init(app);
		stagingBuffer->createBuffer(createInfo, properties);
		return stagingBuffer;
	}



	//void createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkBuffer& buffer) {
	//	VkBufferCreateInfo bufferCreateInfo = {};
	//	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//	bufferCreateInfo.size = deviceSize;
	//	bufferCreateInfo.usage = usageFlags;
	//	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//	if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to create buffer");
	//	}
	//}

	//void allocateBufferMemory(VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer,
	//	VkDeviceMemory& deviceMemory) {
	//	VkMemoryRequirements memoryRequirements;
	//	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	//	VkMemoryAllocateInfo memoryAllocateInfo = {};
	//	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	//	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	//	memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,
	//		propertyFlags);
	//	if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to allocate buffer memory");
	//	}
	//	vkBindBufferMemory(device, buffer, deviceMemory, 0);
	//}

	//void allocateImageMemory(VkMemoryPropertyFlags propertyFlags, VkImage& image,
	//	VkDeviceMemory& deviceMemory) {
	//	VkMemoryRequirements memoryRequirements;
	//	vkGetImageMemoryRequirements(device, image, &memoryRequirements);

	//	VkMemoryAllocateInfo memoryAllocateInfo = {};
	//	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	//	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	//	memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,
	//		propertyFlags);
	//	if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to allocate image memory");
	//	}
	//	vkBindImageMemory(device, image, deviceMemory, 0);
	//}

	//void copyBuffer(VkBuffer srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size) {
	//	VkCommandBuffer commandBuffer = RenderCore::CommandHelper::beginSingleTimeCommands();

	//	VkBufferCopy bufferCopy = {};
	//	bufferCopy.size = size;
	//	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);

	//	CommandHelper::endSingleTimeCommands(commandBuffer);
	//}

	//void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	//{
	//	VkCommandBuffer commandBuffer = CommandHelper::beginSingleTimeCommands();

	//	VkImageMemoryBarrier barrier = {};
	//	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	//	barrier.oldLayout = oldLayout;
	//	barrier.newLayout = newLayout;
	//	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	//	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	//	barrier.image = image;
	//	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	barrier.subresourceRange.baseMipLevel = 0;
	//	barrier.subresourceRange.levelCount = 1;
	//	barrier.subresourceRange.baseArrayLayer = 0;
	//	barrier.subresourceRange.layerCount = 1;
	//	barrier.srcAccessMask = 0;
	//	barrier.dstAccessMask = 0;

	//	VkPipelineStageFlags sourceStage;
	//	VkPipelineStageFlags destinationStage;
	//	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	//	{
	//		barrier.srcAccessMask = 0;
	//		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	//		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	//		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	//	}
	//	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	//	{
	//		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	//		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	//		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	//		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	//	}
	//	else
	//	{
	//		throw std::invalid_argument("unsupported layout transition");
	//	}
	//	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	//	CommandHelper::endSingleTimeCommands(commandBuffer);
	//}

	//void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	//{
	//	VkCommandBuffer commandBuffer = CommandHelper::beginSingleTimeCommands();

	//	VkBufferImageCopy bufferImageCopy = {};
	//	bufferImageCopy.bufferOffset = 0;
	//	bufferImageCopy.bufferRowLength = 0;
	//	bufferImageCopy.bufferImageHeight = 0;

	//	bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	bufferImageCopy.imageSubresource.baseArrayLayer = 0;
	//	bufferImageCopy.imageSubresource.layerCount = 1;
	//	bufferImageCopy.imageSubresource.mipLevel = 0;

	//	bufferImageCopy.imageOffset = { 0,0,0 };
	//	bufferImageCopy.imageExtent = { width,height,1 };
	//	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

	//	CommandHelper::endSingleTimeCommands(commandBuffer);
	//}

	//void createUniformBuffer() {
	//	VkDeviceSize bufferSize = sizeof(ShaderHelper::UniformBufferObject);
	//	uniformBuffers.resize(swapChainImages.size());
	//	uniformBufferMemories.resize(swapChainImages.size());

	//	for (size_t i = 0; i < swapChainImages.size(); i++) {
	//		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniformBuffers[i]);
	//		allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	//			uniformBuffers[i], uniformBufferMemories[i]);
	//	}
	//}


	//void createVertexBuffer(const std::vector<Vertex>& resources, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	//	VkDeviceSize bufferSize = sizeof(resources[0]) * resources.size();
	//	VkBuffer stagingBuffer;
	//	VkDeviceMemory stagingBufferMemory;
	//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer);
	//	allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	//		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
	//		stagingBufferMemory);

	//	void* data;
	//	vkMapMemory(RenderCore::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	//	memcpy(data, resources.data(), (size_t)bufferSize);
	//	vkUnmapMemory(RenderCore::device, stagingBufferMemory);

	//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	//		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, buffer);
	//	allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer,
	//		bufferMemory);
	//	copyBuffer(stagingBuffer, buffer, bufferSize);
	//	vkDestroyBuffer(RenderCore::device, stagingBuffer, nullptr);
	//	vkFreeMemory(RenderCore::device, stagingBufferMemory, nullptr);
	//}

	//void createIndexBuffer(const std::vector<uint16_t>& resources, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	//	VkDeviceSize bufferSize = sizeof(resources[0]) * resources.size();
	//	VkBuffer stagingBuffer;
	//	VkDeviceMemory stagingBufferMemory;
	//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer);
	//	allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	//		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
	//		stagingBufferMemory);
	//	void* data;
	//	vkMapMemory(RenderCore::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	//	memcpy(data, resources.data(), (size_t)bufferSize);
	//	vkUnmapMemory(RenderCore::device, stagingBufferMemory);

	//	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	//		VK_BUFFER_USAGE_INDEX_BUFFER_BIT, buffer);
	//	allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer,
	//		bufferMemory);
	//	copyBuffer(stagingBuffer, buffer, bufferSize);
	//	vkDestroyBuffer(RenderCore::device, stagingBuffer, nullptr);
	//	vkFreeMemory(RenderCore::device, stagingBufferMemory, nullptr);
	//}
}