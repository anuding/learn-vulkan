//
// Created by anuding on 2019/12/24.
//
#include <vulkan/vulkan.h>
#include <vector>
namespace Engine::DebugUtils{
    void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void setupDebugMessenger(bool enableValidationLayers, VkInstance &instance,
                             VkDebugUtilsMessengerEXT &debugUtilsMessengerExt);
    VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger, const
                                       VkAllocationCallbacks *pAllocator);
}
