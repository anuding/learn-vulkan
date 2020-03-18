//
// Created by anuding on 2019/12/24.
//
#include <vulkan/vulkan.h>

namespace Engine::Util {
    class ValidationLayerDebugger {
    public:
        ValidationLayerDebugger(VkInstance instance);
        ~ValidationLayerDebugger();
    private:
        VkDebugUtilsMessengerEXT debugMessenger;
        VkInstance instanceCopy;

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        void setupDebugMessenger(bool enableValidationLayers);

        VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT();
    };
   
}