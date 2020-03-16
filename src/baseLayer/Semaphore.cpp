//
// Created by anuding on 2019/12/21.
//
#include <stdexcept>
#include "Semaphore.h"
#include "VKContext.h"

namespace Engine::RenderCore::SemaphoreHelper {

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateFence(device, &fenceCreateInfo, nullptr, &inFlightFences[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores");
            }
        }
    }

    void init() {
        createSyncObjects();
    }
}
