#define GLFW_INCLUDE_VULKAN
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportdDetails{
    VkSurfaceCapabilitiesKHR capabilitiesKhr;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Application {
private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char *> deviceExtensions ={
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    VkDevice _device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSurfaceKHR surface;

    GLFWwindow *window;
    VkInstance instance;
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

    SwapChainSupportdDetails querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice);
};