//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include "BufferManager.h"
#include "VKContext.h"
#include "Shader.h"
#include "Asset.h"

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

    void BufferManager::createUniformBuffer() {
        VkDeviceSize bufferSize = sizeof(ShaderHelper::UniformBufferObject);
        uniformBuffers.resize(swapChainImages.size());
        uniformBufferMemories.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            allocateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           uniformBuffers[i], uniformBufferMemories[i]);
        }
    }

    void BufferManager::createVertexBuffer(const std::vector<Vertex> &resources, RESOURCE_TYPE resourceType,
                                           VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
        VkDeviceSize bufferSize = sizeof(resources[0]) * resources.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        allocateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                       stagingBufferMemory);
        void *data;
        vkMapMemory(RenderCore::device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, resources.data(), (size_t) bufferSize);
        vkUnmapMemory(RenderCore::device, stagingBufferMemory);

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
        vkDestroyBuffer(RenderCore::device, stagingBuffer, nullptr);
        vkFreeMemory(RenderCore::device, stagingBufferMemory, nullptr);
    }

    void BufferManager::createIndexBuffer(const std::vector<uint16_t> &resources, RESOURCE_TYPE resourceType,
                                          VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
        VkDeviceSize bufferSize = sizeof(resources[0]) * resources.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        allocateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                       stagingBufferMemory);
        void *data;
        vkMapMemory(RenderCore::device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, resources.data(), (size_t) bufferSize);
        vkUnmapMemory(RenderCore::device, stagingBufferMemory);

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
        vkDestroyBuffer(RenderCore::device, stagingBuffer, nullptr);
        vkFreeMemory(RenderCore::device, stagingBufferMemory, nullptr);
    }

    void BufferManager::init(Scene &scene) {
        createVertexBuffer(scene.getGameObjects()[0].getMesh().getVertices(), VERTEX, vertexBuffer, vertexBufferMemory);
        createIndexBuffer(scene.getGameObjects()[0].getMesh().getIndices(), INDEX, indexBuffer, indexBufferMemory);
        createUniformBuffer();
        createTextureBuffer();
    }

    void BufferManager::createTextureBuffer() {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;
        auto m = assetManager.getAssetMap();
        for (auto asset = m.begin(); asset != m.end(); ++asset) {
            auto name = asset->first;
            auto metainfo = asset->second.getMetaInfo();

            allocateBuffer(metainfo.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                           stagingBuffer, stagingMemory);
            void *data;
            vkMapMemory(device, stagingMemory, 0, metainfo.size, 0, &data);
            memcpy(data, asset->second.getData(), static_cast<size_t >(metainfo.size));
            vkUnmapMemory(device, stagingMemory);
            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            VkImageCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = static_cast<uint32_t >(metainfo.width);
            createInfo.extent.height = static_cast<uint32_t >(metainfo.height);
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = 1;
            createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT
                    | VK_IMAGE_USAGE_SAMPLED_BIT;
            createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            createInfo.flags = 0;
            if(vkCreateImage(device,&createInfo, nullptr,&textureImage)!=VK_SUCCESS){
                throw std::runtime_error("failed to create image");
            }


        }
    }
}