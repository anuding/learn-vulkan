#include "DeviceQueueUtil.h"
namespace Engine::Utils {
    const std::vector<const char*> physicalDeviceExtensions = {
       VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    std::vector<QueueFamily> selectedQueueFamilies;

    SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
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

    std::vector<QueueFamily> getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        std::vector<QueueFamily> queueFamilies;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
        VkBool32 presentSupport = false;
        int i = 0;
        for (auto& queueFamilyProperty : queueFamilyProperties) {
            QueueFamily qf;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                qf.abilities.push_back(GRAPHICS);
            if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
                qf.abilities.push_back(COMPUTE);
            if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
                qf.abilities.push_back(TRANSFER);
            if (queueFamilyProperty.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                qf.abilities.push_back(SPARSE);
            if (presentSupport)
                qf.abilities.push_back(PRESENT);
            qf.count = queueFamilyProperty.queueCount;
            qf.index = i;
            queueFamilies.push_back(qf);
            i++;
        }
        return queueFamilies;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        VkBool32 presentSupport = false;
        int i = 0;
        for (auto& qfp : queueFamilyProperties) {
            if (qfp.queueCount > 0 && qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (qfp.queueCount > 0 && presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
            i++;
        }
        return indices;
    }

  /*  VkQueue getQueue(VkDevice device, ) {
        VkQueue q;
        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &q);
        return q;
    }*/

}