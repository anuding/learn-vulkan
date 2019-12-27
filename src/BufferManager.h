//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BUFFERMANAGER_H
#define ENGINE_BUFFERMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "VKContext.h"

namespace Engine::RenderCore::Resource {

    enum RESOURCE_TYPE {
        VERTEX,
        INDEX
    };

    class BufferManager {

    public:
        template<class T>
        void createBuffer(const std::vector<T> &resources, RESOURCE_TYPE resourceType, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory) {
            VkDeviceSize bufferSize = sizeof(resources[0]) * resources.size();
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            allocateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                           stagingBufferMemory);
            void *data;
            vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, resources.data(), (size_t) bufferSize);
            vkUnmapMemory(device, stagingBufferMemory);

            VkBufferUsageFlags bufferUsageFlag;
            if (resourceType == VERTEX)
                bufferUsageFlag = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            if (resourceType == INDEX)
                bufferUsageFlag = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

            allocateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                       bufferUsageFlag,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer,
                           bufferMemory);
            copyBuffer(stagingBuffer, buffer, bufferSize);
            vkDestroyBuffer(device, stagingBuffer, nullptr);
            vkFreeMemory(device, stagingBufferMemory, nullptr);
        }

    private:
        void allocateBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags,
                            VkBuffer &buffer, VkDeviceMemory &deviceMemory);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);

        void copyBuffer(VkBuffer srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);
    };
}


#endif //ENGINE_BUFFERMANAGER_H
