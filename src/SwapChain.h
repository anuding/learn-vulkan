//
// Created by anuding on 2019/11/21.
//

#ifndef ENGINE_SWAPCHAIN_H
#define ENGINE_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore {
    namespace SwapChainHelper {
        struct SwapChainSupportedDetails {
            VkSurfaceCapabilitiesKHR capabilitiesKhr;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    }
#endif //ENGINE_SWAPCHAIN_H
}