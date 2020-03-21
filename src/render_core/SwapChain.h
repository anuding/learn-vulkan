#pragma once
//
// Created by anuding on 2019/11/21.
//
#include <vulkan/vulkan.h>
#include <vector>
#include "VkComp.h"
namespace Engine::RenderCore {
    struct SwapChainSupportedDetails {
        VkSurfaceCapabilitiesKHR capabilitiesKhr;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class SwapChain : public VkComp<VkSwapchainKHR> {
    public:
        SwapChain() = default;
        ~SwapChain();
        // Inherited via VkComp
        virtual void init(Application* app) override;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        uint32_t imageCount;
    private:


        void createSwapChain(uint32_t width, uint32_t height);

        void createSwapChainImages();

        void createSwapChainImageViews();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);
    };
}
