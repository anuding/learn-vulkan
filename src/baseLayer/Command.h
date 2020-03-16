//
// Created by anuding on 2019/12/21.
//
#include <vulkan/vulkan.h>
#include <vector>
#include "Scene.h"

namespace Engine::RenderCore::CommandHelper {
    void init();

    void createCommandPool();

    void allocateCommandBuffers();

    void recordCommandBuffers(Scene &scene);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleCommandBuffer(VkCommandBuffer commandBuffer);
}