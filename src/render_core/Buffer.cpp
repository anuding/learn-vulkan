#pragma once
#include "Buffer.h"
#include <stdexcept>
#include "Application.h"
namespace Engine::RenderCore {
	Buffer::~Buffer()
	{
		vkDestroyBuffer(this->app->device.get(), this->get(), nullptr);
	}

	void Buffer::init(Application* app)
	{
		this->app = app;
	}

	void Buffer::createBuffer(const VkBufferCreateInfo& createInfo, const VkMemoryPropertyFlags& properties) {
		if (vkCreateBuffer(this->app->device.get(), &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create Buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(this->app->device.get(), this->get(), &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findBufferMemoryType(memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(this->app->device.get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(this->app->device.get(), this->get(), bufferMemory, 0);
	}
	void Buffer::updateBufferMemory(unsigned char* bufferData, uint32_t size)
	{
		void* data;
		vkMapMemory(this->app->device.get(), bufferMemory, 0, size, 0, &data);
		memcpy(data, bufferData, static_cast<size_t>(size));
		vkUnmapMemory(this->app->device.get(), bufferMemory);
	}
	uint32_t Buffer::findBufferMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
}