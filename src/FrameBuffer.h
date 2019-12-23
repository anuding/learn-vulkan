//
// Created by anuding on 2019/12/21.
//


#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore {
    namespace FrameBufferHelper {
        void createFrameBuffers(VkDevice &device, std::vector<VkFramebuffer> &swapChainFrameBuffers,
                                std::vector<VkImageView> &swapChainViews,
                                VkRenderPass &renderPass, VkExtent2D &swapChainExtent);
    }
}