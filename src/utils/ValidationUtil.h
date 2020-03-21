#pragma once
//
// Created by anuding on 2019/12/24.
//
#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::Utils {
    const std::vector<const char*> validationLayers = {
        //"VK_LAYER_KHRONOS_validation"
    "VK_LAYER_LUNARG_standard_validation"
    };

    extern void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    
    extern bool enableValidationLayers;
    class ValidationLayerDebugger {
    public:
        ValidationLayerDebugger(VkInstance instance);
        ~ValidationLayerDebugger();
    private:
        VkDebugUtilsMessengerEXT debugMessenger;
        VkInstance instanceCopy;

        void setupDebugMessenger();

        VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT();
    };
   
}
