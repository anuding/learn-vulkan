//
// Created by anuding on 2019/12/26.
//

#ifndef LEARN_VULKAN_VKCONTEXT_H
#define LEARN_VULKAN_VKCONTEXT_H

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore {
    extern VkDevice device;
    extern VkPhysicalDevice physicalDevice;
    extern VkBuffer vertexBuffer;
    extern VkDeviceMemory vertexBufferMemory;

    extern std::vector<VkCommandBuffer> commandBuffers;
    extern VkDebugUtilsMessengerEXT debugMessenger;
    extern VkQueue graphicsQueue;
    extern VkQueue presentQueue;
    extern VkSwapchainKHR swapChain;
    extern VkSurfaceKHR surface;
    extern std::vector<VkImage> swapChainImages;
    extern std::vector<VkImageView> swapChainImageViews;
    extern VkFormat swapChainImageFormat;
    extern VkExtent2D swapChainExtent;
    extern VkRenderPass renderPass;
    extern VkPipelineLayout pipelineLayout;
    extern VkPipeline graphicsPipeline;
    extern std::vector<VkFramebuffer> swapChainFrameBuffers;
    extern VkCommandPool commandPool;
    extern std::vector<VkSemaphore> imageAvailableSemaphores;
    extern std::vector<VkSemaphore> renderFinishedSemaphores;
    extern size_t currentFrame;
    extern std::vector<VkFence> inFlightFences;
    extern GLFWwindow *window;
    extern VkInstance instance;
}


#endif //LEARN_VULKAN_VKCONTEXT_H
