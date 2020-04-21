//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BufferHelper_H
#define ENGINE_BufferHelper_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Buffer.h"
#include <memory>

namespace Engine::RenderCore::BufferFactory {

    std::unique_ptr<Buffer> createStagingBuffer(Application* app, VkDeviceSize size);

    //void createVertexBuffer(const std::vector<Vertex> &resources, VkBuffer &buffer,
    //                        VkDeviceMemory &bufferMemory);

    //void createIndexBuffer(const std::vector<uint16_t> &resources, VkBuffer &buffer,
    //                       VkDeviceMemory &bufferMemory);

    //void createUniformBuffer();

    //void createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkBuffer &buffer);

    //void allocateBufferMemory(VkMemoryPropertyFlags propertyFlags, VkBuffer &buffer,
    //                          VkDeviceMemory &deviceMemory);

    //void allocateImageMemory(VkMemoryPropertyFlags propertyFlags, VkImage &image,
    //                         VkDeviceMemory &deviceMemory);

    //void copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);

    //void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    //void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
}


#endif //ENGINE_BufferHelper_H
