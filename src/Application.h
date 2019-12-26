#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>
#include "BufferManager.h"
#include "VKContext.h"
#include <chrono>

namespace Engine::RenderCore {
    class Application {
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
        const std::vector<const char *> _validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char *> _deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    public:
        Resource::BufferManager bufferManager;
        std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

    public:
        Application();
        void run();

        void initWindow();

        void initVulkan();

        void mainLoop();

        void cleanUp();

        void createInstance();

        void checkValidationLayerSupport();

        std::vector<const char *> getRequiredExtensions();

        void checkInstanceExtensionsSupport(const std::vector<const char *> &requiredExtensions);

        void pickPhysicalDevice();

        void checkPhysicalDeviceSupport(std::vector<VkPhysicalDevice> &vkPhysicalDevices);

        bool isDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);

        void createLogicalDevice();

        void createSurface();

        bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        void createSwapChain();

        void createImageViews();

        virtual void drawFrame();

        virtual void update();
    };
}