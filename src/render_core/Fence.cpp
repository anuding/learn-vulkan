#pragma once
//
// Created by anuding on 2019/12/21.
//
#include <stdexcept>
#include "Fence.h"
#include "Application.h"

namespace Engine::RenderCore {

    const int MAX_FRAMES_IN_FLIGHT = 2;
    Fence::~Fence()
    {
        for (auto& f : this->get()) {
            vkDestroyFence(app->device.get(), f, nullptr);
        }
    }
    void Fence::init(Application* app)
    {
        this->app = app;
        createFences();
    }
    void Fence::createFences() {
        this->get().resize(MAX_FRAMES_IN_FLIGHT);
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateFence(app->device.get(), &fenceCreateInfo, nullptr, &this->get()[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create fences");
            }
        }
    }
}
