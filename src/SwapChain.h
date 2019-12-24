//
// Created by anuding on 2019/11/21.
//


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
        SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice &vkPhysicalDevice, VkSurfaceKHR surfaceKhr);
    }
}