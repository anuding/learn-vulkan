//
// Created by anuding on 2019/12/24.
//

#include "Queue.h"
namespace Engine::RenderCore::Queue{
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &vkPhysicalDevice, VkSurfaceKHR &surfaceKhr) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        VkBool32 presentSupport = false;
        int i = 0;
        for (auto &qfp: queueFamilyProperties) {
            if (qfp.queueCount > 0 && qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, surfaceKhr, &presentSupport);
            if (qfp.queueCount > 0 && presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
            i++;
        }
        return indices;
    }

}