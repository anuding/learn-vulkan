#pragma once
//
// Created by anuding on 2019/12/21.
//

#include <vector>
#include <vulkan/vulkan.h>
#include "VkComp.h"

namespace Engine::RenderCore {
    class Pipeline : public VkComp<VkPipeline> {
    public:
        Pipeline() = default;
        ~Pipeline();
        virtual void init(Application* app) override;
    private:
        void createGraphicsPipelines();
    };
}
