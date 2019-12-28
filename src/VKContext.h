//
// Created by anuding on 2019/12/26.
//

#ifndef LEARN_VULKAN_VKCONTEXT_H
#define LEARN_VULKAN_VKCONTEXT_H

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore {

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    extern VkDevice device;
    extern VkPhysicalDevice physicalDevice;
    extern VkBuffer vertexBuffer;
    extern VkDeviceMemory vertexBufferMemory;
    extern VkBuffer indexBuffer;
    extern VkDeviceMemory indexBufferMemory;
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
    extern VkDescriptorSetLayout descriptorSetLayout;
    extern VkDescriptorPool descriptorPool;
    extern std::vector<VkDescriptorSet> descriptorSets;

    extern std::vector<VkBuffer> uniformBuffers;
    extern std::vector<VkDeviceMemory> uniformBufferMemories;

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
