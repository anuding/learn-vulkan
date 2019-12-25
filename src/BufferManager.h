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
                std::vector<Vertex> vertices
        );

    private:
        uint32_t findMemoryType( VkPhysicalDevice &physicalDevice,
                                 VkDevice &device,uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);
    };
}


#endif //ENGINE_BUFFERMANAGER_H
