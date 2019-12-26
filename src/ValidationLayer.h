//
// Created by anuding on 2019/12/24.
//
#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore::DebugUtils {
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    void setupDebugMessenger(bool enableValidationLayers);

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void
    DestroyDebugUtilsMessengerEXT( );
}
