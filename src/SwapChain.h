//
// Created by anuding on 2019/11/21.
//


#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore::SwapChainHelper {

    struct SwapChainSupportedDetails {
        VkSurfaceCapabilitiesKHR capabilitiesKhr;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void createSwapChain(uint32_t width, uint32_t height);

    void createImageViews();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height);

    SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);
}
