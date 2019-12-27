//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_DEVICE_H
#define ENGINE_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore::DeviceHelper {

    void pickPhysicalDevice();

    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice);

    bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);


}


#endif //ENGINE_DEVICE_H
