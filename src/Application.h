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

namespace Engine::RenderCore {
    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };


    class Application {
    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
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
        std::vector<VkFramebuffer > _swapChainFramebuffers;
        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer > _commandBuffers;

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

        void setupDebugMessenger();

        VkResult CreateDebugUtilsMessengerEXT(
                VkInstance instance,
                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                const VkAllocationCallbacks *pAllocator,
                VkDebugUtilsMessengerEXT *pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks *pAllocator);

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        void pickPhysicalDevice();

        void checkPhysicalDeviceSupport(std::vector<VkPhysicalDevice> &vkPhysicalDevices);

        bool isDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice vkPhysicalDevice);

        void createLogicalDevice();

        void createSurface();

        bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        SwapChainHelper::SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice);

        void createSwapChain();

        void createImageViews();

        void createGraphicsPipelines();

        void createRenderPass();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffers();
    };
}