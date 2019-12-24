//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include "BufferManager.h"

namespace Engine::RenderCore::Resource {

    void BufferManager::createVertexBuffer(
            VkPhysicalDevice &physicalDevice,
            VkDevice &device,
            VkBuffer &vertexBuffer,
            VkDeviceMemory &vertexBufferMemory,
            std::vector<Vertex> &vertices
    ) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = sizeof(vertices);
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(device, vertexBuffer, &memoryRequirements);

        VkMemoryAllocateInfo allocateInfo = {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, device,
                                                      memoryRequirements.memoryTypeBits,
                                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if(vkAllocateMemory(device,&allocateInfo, nullptr,&vertexBufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate memory");
        }
        vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
        void *data;
        vkMapMemory(device,vertexBufferMemory,0,bufferCreateInfo.size,0,&data);
        memcpy(data,vertices.data(),(size_t)bufferCreateInfo.size);
        vkUnmapMemory(device,vertexBufferMemory);
    }

    uint32_t BufferManager::findMemoryType(VkPhysicalDevice &physicalDevice,
                                           VkDevice &device, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags) {
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
}