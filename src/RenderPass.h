//
// Created by anuding on 2019/12/21.
//

#include <vulkan/vulkan.h>

namespace Engine::RenderCore {
    namespace RenderPassHelper {
        void createRenderPass(VkDevice &device, VkFormat &swapChainImageFormat, VkRenderPass &renderPass);
    }
}