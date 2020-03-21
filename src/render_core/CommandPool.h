#pragma once
//
// Created by anuding on 2019/12/21.
//
#include <vulkan/vulkan.h>
#include <vector>
#include "VkComp.h"

namespace Engine::RenderCore {

    class CommandPool : public VkComp<VkCommandPool> {
    public:
        CommandPool() = default;
        ~CommandPool();
        // Inherited via VkComp
        virtual void init(Application* app) override;

        std::vector<VkCommandBuffer> commandBuffers;
    private:

        void createCommandPool();

        void allocateCommandBuffers();

        void recordCommandBuffers();

        VkCommandBuffer beginSingleTimeCommands();

        void endSingleCommandBuffer(VkCommandBuffer commandBuffer);

    };
}