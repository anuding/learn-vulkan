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
        void createVertexBuffer(const std::vector<Vertex> &vertices);


    private:
        void createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags,
                          VkBuffer &buffer, VkDeviceMemory &deviceMemory);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

        void copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);
    };
}


#endif //ENGINE_BUFFERMANAGER_H
