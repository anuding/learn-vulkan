//
// Created by anuding on 2019/11/21.
//

#include <limits>
#include "SwapChain.h"
#include "VKContext.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace Engine::RenderCore {
    namespace SwapChainHelper {

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

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                VkExtent2D actualExtent = {width, height};
                actualExtent.width = std::max(capabilities.minImageExtent.width,
                                              std::min(capabilities.maxImageExtent.width, actualExtent.width));
                actualExtent.height = std::max(capabilities.minImageExtent.height,
                                               std::min(capabilities.maxImageExtent.height, actualExtent.height));
                return actualExtent;
            }
        }

        SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) {
            SwapChainSupportedDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilitiesKhr);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount,
                                                          details.presentModes.data());
            }
            return details;
        }
    }

}