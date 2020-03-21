
#pragma once
//
// Created by anuding on 2019/12/21.
//

#ifndef ENGINE_FENCE_H
#define ENGINE_FENCE_H
#include <vector>
#include <vulkan/vulkan.h>
#include "VkComp.h"

namespace Engine::RenderCore {
    class Fence : public VkComp<std::vector<VkFence>> {
    public:
        Fence() = default;
        ~Fence();
        // Inherited via VkComp
        virtual void init(Application* app) override;
    private:
        void createFences();
    };

}
#endif