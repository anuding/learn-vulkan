//
// Created by anuding on 2019/12/21.
//
#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore::CommandHelper {
    void createCommandPool(VkPhysicalDevice &physicalDevice, VkDevice &device, VkCommandPool &commandPool,
                           VkSurfaceKHR &surfaceKhr);

    void createCommandBuffers(
            VkDevice &device,
            std::vector<VkCommandBuffer> &commandBuffers,
            std::vector<VkFramebuffer> &_swapChainFrameBuffers,
            VkCommandPool &commandPool,
            VkRenderPass &renderPass,
            VkExtent2D &extent2D,
            VkPipeline &graphicsPipeline,
            VkBuffer &vertexBuffer,
            uint32_t size);
}