//
// Created by anuding on 2019/12/21.
//

#include <stdexcept>
#include "CommandBuffer.h"
namespace Engine::RenderCore{
    namespace CommandBufferHelper{
        void createCommandBuffers() {
            _commandBuffers.resize(_swapChainFrameBuffers.size());
            VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.commandPool = _commandPool;
            commandBufferAllocateInfo.commandBufferCount = (uint32_t) _commandBuffers.size();
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if (vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, _commandBuffers.data()) != VK_SUCCESS) {
                throw std::runtime_error("failed to create command buffers");
            }

            for (size_t i = 0; i < _commandBuffers.size(); i++) {
                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
                beginInfo.pInheritanceInfo = nullptr;

                VkRenderPassBeginInfo renderPassBeginInfo = {};
                renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassBeginInfo.renderPass = _renderPass;
                renderPassBeginInfo.clearValueCount = 1;
                renderPassBeginInfo.framebuffer = _swapChainFrameBuffers[i];
                renderPassBeginInfo.renderArea.offset = {0, 0};
                renderPassBeginInfo.renderArea.extent = _swapChainExtent;
                VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};
                renderPassBeginInfo.pClearValues = &clearValue;


                if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                    throw std::runtime_error("failed to begin");
                }
                vkCmdBeginRenderPass(_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
                vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);
                vkCmdEndRenderPass(_commandBuffers[i]);
                if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to record command buffer");
                }
            }
        }
    }
}