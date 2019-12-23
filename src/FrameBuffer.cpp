//
// Created by anuding on 2019/12/21.
//

#include <stdexcept>
#include "FrameBuffer.h"

namespace Engine::RenderCore {
    namespace FrameBufferHelper {
        void createFrameBuffers(VkDevice &device, std::vector<VkFramebuffer> &swapChainFrameBuffers,
                                std::vector<VkImageView> &swapChainViews,
                                VkRenderPass &renderPass, VkExtent2D &swapChainExtent) {
            swapChainFrameBuffers.resize(swapChainViews.size());
            for (size_t i = 0; i < swapChainViews.size(); i++) {
                VkImageView attachments[] = {
                        swapChainViews[i]
                };

                VkFramebufferCreateInfo framebufferCreateInfo = {};
                framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferCreateInfo.renderPass = renderPass;
                framebufferCreateInfo.pAttachments = attachments;
                framebufferCreateInfo.attachmentCount = 1;
                framebufferCreateInfo.width = swapChainExtent.width;
                framebufferCreateInfo.height = swapChainExtent.height;
                framebufferCreateInfo.layers = 1;

                if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &swapChainFrameBuffers[i]) !=
                    VK_SUCCESS) {
                    throw std::runtime_error("failed to create framebuffer");
                }
            }
        }
    }
}