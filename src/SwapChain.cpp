//
// Created by anuding on 2019/11/21.
//

#include <limits>
#include "SwapChain.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace Engine::RenderCore {
    namespace SwapChainHelper {
        const uint32_t WIDTH = 720;
        const uint32_t HEIGHT = 480;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
            if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
                return {VK_FORMAT_B8G8R8A8_UNORM,
                        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
            for (auto &format: availableFormats) {
                if (format.format == VK_FORMAT_R8G8B8_UNORM
                    && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
                    return format;
            }
            return availableFormats[0];
        }

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
            VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
            for (auto &mode:availablePresentModes) {
                if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                    return mode;
                else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
                    bestMode = mode;
            }
            return bestMode;
        }

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                VkExtent2D actualExtent = {WIDTH, HEIGHT};
                actualExtent.width = std::max(capabilities.minImageExtent.width,
                                              std::min(capabilities.maxImageExtent.width, actualExtent.width));
                actualExtent.height = std::max(capabilities.minImageExtent.height,
                                               std::min(capabilities.maxImageExtent.height, actualExtent.height));
                return actualExtent;
            }
        }
    }

}