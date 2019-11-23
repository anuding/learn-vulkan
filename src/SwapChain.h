//
// Created by anuding on 2019/11/21.
//

#ifndef ENGINE_SWAPCHAIN_H
#define ENGINE_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>

struct SwapChainSupportdDetails{
    VkSurfaceCapabilitiesKHR capabilitiesKhr;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain {
public:
    VkSurfaceKHR surface;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
};


#endif //ENGINE_SWAPCHAIN_H
