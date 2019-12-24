//
// Created by anuding on 2019/12/21.
//

#include <stdexcept>
#include "CommandBuffer.h"
#include "Queue.h"

namespace Engine::RenderCore::CommandHelper {
    void createCommandPool(VkPhysicalDevice &physicalDevice, VkDevice &device, VkCommandPool &commandPool,
                           VkSurfaceKHR &surfaceKhr) {
        Queue::QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(
                physicalDevice, surfaceKhr);
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        commandPoolCreateInfo.flags = 0;

        if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool");
        }
    }

    void createCommandBuffers(
            VkDevice &device,
            std::vector<VkCommandBuffer> &commandBuffers,
            std::vector<VkFramebuffer> &_swapChainFrameBuffers,
            VkCommandPool &commandPool,
            VkRenderPass &renderPass,
            VkExtent2D &extent2D,
            VkPipeline &graphicsPipeline,
            VkBuffer &vertexBuffer) {
        commandBuffers.resize(_swapChainFrameBuffers.size());
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.commandBufferCount = (uint32_t) commandBuffers.size();
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command buffers");
        }
////////////////////////////////////////////////
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = renderPass;
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.framebuffer = _swapChainFrameBuffers[i];
            renderPassBeginInfo.renderArea.offset = {0, 0};
            renderPassBeginInfo.renderArea.extent = extent2D;
            VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassBeginInfo.pClearValues = &clearValue;


            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin");
            }
            vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers,
                                   offsets);
            vkCmdDraw(commandBuffers[i], static_cast<uint32_t >(3), 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer");
            }
        }


    }
}