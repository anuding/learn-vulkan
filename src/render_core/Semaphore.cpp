#pragma once
//
// Created by anuding on 2019/12/21.
//
#include <stdexcept>
#include "Semaphore.h"
#include "Application.h"

namespace Engine::RenderCore {

    const int MAX_FRAMES_IN_FLIGHT = 2;
    Semaphore::~Semaphore()
    {
        for (auto& s : this->get()) {
            vkDestroySemaphore(app->device.get(), s, nullptr);
        }
    }
    void Semaphore::init(Application* app)
    {
        this->app = app;
        createSyncObjects();
    }
    void Semaphore::createSyncObjects() {
        this->get().resize(MAX_FRAMES_IN_FLIGHT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
       
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(app->device.get(), &semaphoreCreateInfo, nullptr, &this->get()[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores");
            }
        }
    }
}
