
#pragma once
//
// Created by anuding on 2019/12/21.
//

#ifndef ENGINE_SEMAPHORE_H
#define ENGINE_SEMAPHORE_H
#include <vector>
#include <vulkan/vulkan.h>
#include "VkComp.h"

namespace Engine::RenderCore {
    class Semaphore : public VkComp<std::vector<VkSemaphore>> {
    public:
        Semaphore() = default;
        ~Semaphore();
        // Inherited via VkComp
        virtual void init(Application* app) override;
    private:
        void createSyncObjects();
    };

}
#endif