#define GLFW_INCLUDE_VULKAN
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "SwapChain.h"
#include "Queue.h"
namespace Engine::RenderCore {
    class Application {
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
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _device;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkSwapchainKHR _swapChain;
        VkSurfaceKHR _surface;
        std::vector<VkImage> _swapChainImages;
        std::vector<VkImageView> _swapChainImageViews;
        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;
        VkRenderPass _renderPass;
        VkPipelineLayout _pipelineLayout;
        VkPipeline  _graphicsPipeline;
        std::vector<VkFramebuffer > _swapChainFrameBuffers;
        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer > _commandBuffers;

        std::vector<VkSemaphore > _imageAvailableSemaphores;
        std::vector<VkSemaphore > _renderFinishedSemaphores;
        size_t _currentFrame = 0;
        std::vector<VkFence > _inFlightFences;
        GLFWwindow *_window;
        VkInstance _instance;
    public:
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

        void drawFrame();

    };
}