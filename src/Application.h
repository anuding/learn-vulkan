#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "BufferManager.h"
#include <chrono>

namespace Engine::RenderCore {
    class Application {
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    protected:
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _device;
        VkBuffer _vertexBuffer=VK_NULL_HANDLE;
        VkDeviceMemory _vertexBufferMemory=VK_NULL_HANDLE;
        Resource::BufferManager bufferManager;
        std::vector<VkCommandBuffer > _commandBuffers;

    public:
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

        std::vector<VkSemaphore > _imageAvailableSemaphores;
        std::vector<VkSemaphore > _renderFinishedSemaphores;
        size_t _currentFrame = 0;
        std::vector<VkFence > _inFlightFences;
        GLFWwindow *_window;
        VkInstance _instance;
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