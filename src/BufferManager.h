//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BUFFERMANAGER_H
#define ENGINE_BUFFERMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "Scene.h"
#include "VKContext.h"

namespace Engine::RenderCore::BufferManager {
    enum RESOURCE_TYPE {
        VERTEX,
        INDEX,
        UNIFORM
    };

    void init(Scene &scene);

    void createTextureBuffer();

    void createTextureView();

    void createTextureSampler();

    void createVertexBuffer(const std::vector<Vertex> &resources, VkBuffer &buffer,
                            VkDeviceMemory &bufferMemory);

    void createIndexBuffer(const std::vector<uint16_t> &resources, VkBuffer &buffer,
                           VkDeviceMemory &bufferMemory);

    void createUniformBuffer();

    void createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkBuffer &buffer);

    void allocateBufferMemory(VkMemoryPropertyFlags propertyFlags, VkBuffer &buffer,
                              VkDeviceMemory &deviceMemory);

    void allocateImageMemory(VkMemoryPropertyFlags propertyFlags, VkImage &image,
                             VkDeviceMemory &deviceMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

    template<class T>
    void testFunc1(T b) {
        auto a = Engine::RenderCore::device;
    }

    void copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
}


#endif //ENGINE_BUFFERMANAGER_H
