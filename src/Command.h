//
// Created by anuding on 2019/12/21.
//
#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore::CommandHelper {
    void createCommandPool();

    void createCommandBuffers(
            uint32_t vertexArrayLength);
}