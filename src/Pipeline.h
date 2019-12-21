//
// Created by anuding on 2019/12/21.
//

#include <vector>
#include <vulkan/vulkan.h>

namespace Engine::RenderCore {
    namespace PipelineHelper {
        void createGraphicsPipelines(VkDevice &device, VkExtent2D &swapChainExtent, VkPipelineLayout &pipelineLayout,
                                     VkRenderPass &renderPass, VkPipeline &graphicsPipeline);
    }
}