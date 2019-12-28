//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include "BufferManager.h"
#include "VKContext.h"
#include "Shader.h"

namespace Engine::RenderCore::Resource {
    uint32_t BufferManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags) {
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) &&
                (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
                return i;
        }
        throw std::runtime_error("failed to find suitable memory type");
        return 0;
    }

    void BufferManager::allocateBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags,
                                       VkMemoryPropertyFlags propertyFlags, VkBuffer &buffer,
                                       VkDeviceMemory &deviceMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = deviceSize;
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,
                                                            propertyFlags);
        if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory");
        }
        vkBindBufferMemory(device, buffer, deviceMemory, 0);
    }

    void BufferManager::copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = commandPool;
        allocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy bufferCopy = {};
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    void BufferManager::createLocalBuffer() {
        VkDeviceSize bufferSize = sizeof(ShaderHelper::UniformBufferObject);
        uniformBuffers.resize(swapChainImages.size());
        uniformBufferMemories.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            allocateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           uniformBuffers[i], uniformBufferMemories[i]);
        }
    }
}