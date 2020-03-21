#pragma once

//
// Created by anuding on 2019/12/21.
//

#include <stdexcept>
#include "FrameBuffer.h"
#include "Application.h"

namespace Engine::RenderCore {
    FrameBuffer::~FrameBuffer()
    {
        for (auto& fb : this->get()) {
            vkDestroyFramebuffer(app->device.get(), fb, nullptr);
        }
    }
    void FrameBuffer::init(Application* app)
    {
        this->app = app;
        createFrameBuffers();
    }
    void FrameBuffer::createFrameBuffers() {
        this->get().resize(app->swapChain.imageCount);
        for (size_t i = 0; i < app->swapChain.imageCount; i++) {
            VkImageView attachments[] = {
                    app->swapChain.swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = app->renderPass.get();
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.width = app->swapChain.swapChainExtent.width;
            framebufferCreateInfo.height = app->swapChain.swapChainExtent.height;
            framebufferCreateInfo.layers = 1;

            if (vkCreateFramebuffer(app->device.get(), &framebufferCreateInfo, nullptr, &this->get()[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer");
            }
        }
    }
}
