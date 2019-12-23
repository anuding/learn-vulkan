//
// Created by anuding on 2019/12/24.
//
#include "vulkan/vulkan.h"
#include "vector"

namespace Engine::RenderCore::Queue {
    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &vkPhysicalDevice, VkSurfaceKHR &surfaceKhr);

    class Queue {

    };


}

