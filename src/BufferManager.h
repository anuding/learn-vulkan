//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BUFFERMANAGER_H
#define ENGINE_BUFFERMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Vertex.h"

namespace Engine::RenderCore::Resource {
    class BufferManager {
    public:
        void createVertexBuffer(
                VkPhysicalDevice &physicalDevice,
                VkDevice &device,
                VkBuffer &vertexBuffer,
                VkDeviceMemory &vertexBufferMemory,
                const std::vector<Vertex> &vertices,
                VkCommandPool &commandPool, VkQueue &queue
        );


    private:
        void createBuffer(VkPhysicalDevice &physicalDevice, VkDevice &device, VkDeviceSize deviceSize,
                          VkBufferUsageFlags usageFlags,
                          VkMemoryPropertyFlags propertyFlags, VkBuffer &buffer, VkDeviceMemory &deviceMemory);

        uint32_t findMemoryType(VkPhysicalDevice &physicalDevice,
                                VkDevice &device, uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

        void copyBuffer(VkDevice &device, VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size,
                        VkCommandPool &commandPool, VkQueue &queue);
    };
}


#endif //ENGINE_BUFFERMANAGER_H
