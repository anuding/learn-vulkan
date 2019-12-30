//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BUFFERMANAGER_H
#define ENGINE_BUFFERMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "VKContext.h"
#include "Scene.h"
namespace Engine::RenderCore::Resource::BufferManager {

    enum RESOURCE_TYPE {
        VERTEX,
        INDEX,
        UNIFORM
    };

    void init(Scene &scene);

    void createTextureBuffer();

    void createVertexBuffer(const std::vector<Vertex> &resources, RESOURCE_TYPE resourceType, VkBuffer &buffer,
                            VkDeviceMemory &bufferMemory);

    void createIndexBuffer(const std::vector<uint16_t> &resources, RESOURCE_TYPE resourceType, VkBuffer &buffer,
                           VkDeviceMemory &bufferMemory);

    void createUniformBuffer();

    void allocateBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags,
                        VkBuffer &buffer, VkDeviceMemory &deviceMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);

}


#endif //ENGINE_BUFFERMANAGER_H
