#pragma once
//
// Created by anuding on 2019/12/21.
//


#include <vulkan/vulkan.h>
#include <vector>
#include "VkComp.h"
namespace Engine::RenderCore {
    class FrameBuffer : public VkComp<std::vector<VkFramebuffer>>{
    public:
        FrameBuffer() = default;
        ~FrameBuffer();
        // Inherited via VkComp
        virtual void init(Application* app) override;
    private:
        void createFrameBuffers();
    };
}
