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

	std::unique_ptr<Buffer> createMVPUniformBuffer(Application* app)
	{
		VkDeviceSize bufferSize = sizeof(UBO_MVP);
		std::unique_ptr<Buffer> uniformBuffer(new Buffer());

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = bufferSize;
		createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		uniformBuffer->init(app);
		uniformBuffer->createBuffer(createInfo, properties);
		return uniformBuffer;		
	}






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