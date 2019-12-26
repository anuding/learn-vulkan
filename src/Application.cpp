#include <vector>
#include <set>
#include "Application.h"
#include "Shader.h"
#include "Semaphore.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "Command.h"
#include "ValidationLayer.h"
#include "SwapChain.h"
#include "Queue.h"
#include <thread>

namespace Engine::RenderCore {

    Application::Application() {
        initWindow();
        initVulkan();
    }

    void Application::run() {

        mainLoop();
        cleanUp();
    }


    void Application::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "EngineTest", nullptr, nullptr);
    }

    void Application::initVulkan() {
        createInstance();
        DebugUtils::setupDebugMessenger(enableValidationLayers);

        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();

        RenderPassHelper::createRenderPass();

        PipelineHelper::createGraphicsPipelines();

        FrameBufferHelper::createFrameBuffers(swapChainFrameBuffers, swapChainImageViews);

        CommandHelper::createCommandPool();


        SemaphoreHelper::createSyncObjects(imageAvailableSemaphores, renderFinishedSemaphores,
                                           inFlightFences, MAX_FRAMES_IN_FLIGHT);


    }

    void Application::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            a = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> work_time = a - b;
            if (work_time.count() < 17) {
                std::chrono::duration<double, std::milli> delta_ms(17 - work_time.count());
                auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
                std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
            }
            b = std::chrono::system_clock::now();

            update();
            drawFrame();
        }
        vkDeviceWaitIdle(device);
    }

    void Application::cleanUp() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(device, commandPool, nullptr);
        for (auto framebuffer: swapChainFrameBuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        for (auto imageView: swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
        //this is a red line
        vkDestroyDevice(device, nullptr);
        if (enableValidationLayers)
            DebugUtils::DestroyDebugUtilsMessengerEXT();

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::createInstance() {
        if (enableValidationLayers)
            checkValidationLayerSupport();
        VkInstanceCreateInfo instanceCreateInfo = {};
        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = "EngineTest";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = "No Engine";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.apiVersion = VK_API_VERSION_1_0;


        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;


        auto requiredExtensions = getRequiredExtensions();
        checkInstanceExtensionsSupport(requiredExtensions);
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t >(requiredExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoExt;
        if (enableValidationLayers) {
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = _validationLayers.data();
            DebugUtils::populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfoExt);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugUtilsMessengerCreateInfoExt;
        } else {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

    }

    std::vector<const char *> Application::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> extensions(glfwExtensions,
                                             glfwExtensions + glfwExtensionCount);
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    void Application::checkInstanceExtensionsSupport(const std::vector<const char *> &requiredExtensions) {
        uint32_t supportedExtensionsCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, supportedExtensions.data());

        for (int i = 0; i < requiredExtensions.size(); i++) {
            bool triggeer = false;
            for (auto &item: supportedExtensions)
                if (std::string(requiredExtensions[i]) == item.extensionName)
                    triggeer = true;
            if (!triggeer)
                throw std::runtime_error("GLFW needs extension:" + std::string(requiredExtensions[i]));
        }
    }

    void Application::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> supportlayerProperties(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, supportlayerProperties.data());
        for (auto &layer:_validationLayers) {
            bool trigger = false;
            for (auto &supportLayer: supportlayerProperties) {
                if (std::string(layer) == supportLayer.layerName)
                    trigger = true;
            }
            if (!trigger)
                throw std::runtime_error("validation layers requested, but not available!");
        }
    }

    void Application::pickPhysicalDevice() {
        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("No physical device!");
        std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, vkPhysicalDevices.data());

        for (const auto &device: vkPhysicalDevices) {
            if (isDeviceSuitable(device))
                physicalDevice = device;
        }
        if (physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("These devices are not suitable!");
    }

    void Application::checkPhysicalDeviceSupport(std::vector<VkPhysicalDevice> &vkPhysicalDevices) {
        for (const auto &device: vkPhysicalDevices) {
            if (isDeviceSuitable(device))
                physicalDevice = device;
        }
        if (physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("These devices are not suitable!");
    }

    bool Application::isDeviceSuitable(VkPhysicalDevice physicalDevice) {
        Queue::QueueFamilyIndices indices = Queue::findQueueFamilies(physicalDevice);
        bool isExtensionsSupported = checkPhysicalDeviceExtensionSupport(physicalDevice);

        bool swapChainAdequate = false;
        if (isExtensionsSupported) {
            SwapChainHelper::SwapChainSupportedDetails details = SwapChainHelper::querySwapChainSupport(physicalDevice);
            swapChainAdequate = !details.formats.empty()
                                && !details.presentModes.empty();
        }
        return Queue::findQueueFamilies(physicalDevice).isComplete()
               && isExtensionsSupported
               && swapChainAdequate;
    }

    void Application::createLogicalDevice() {
        Queue::QueueFamilyIndices indices = Queue::findQueueFamilies(physicalDevice);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

        float queuePriority = 1.0f;
        for (int queueFamily:uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = {};
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t >(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &vkPhysicalDeviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t >(_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = _deviceExtensions.data();
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t > (_validationLayers.size());
            createInfo.ppEnabledLayerNames = _validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }
        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
    }

    void Application::createSurface() {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    bool Application::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

        for (const auto &extension:availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
    }


    void Application::createSwapChain() {
        SwapChainHelper::SwapChainSupportedDetails details = SwapChainHelper::querySwapChainSupport(physicalDevice);
        VkSurfaceFormatKHR surfaceFormatKhr = SwapChainHelper::chooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentModeKhr = SwapChainHelper::chooseSwapPresentMode(details.presentModes);
        VkExtent2D extent2D = SwapChainHelper::chooseSwapExtent(details.capabilitiesKhr, WIDTH, HEIGHT);

        uint32_t imageCount = details.capabilitiesKhr.minImageCount + 1;
        if (details.capabilitiesKhr.maxImageCount > 0
            && imageCount > details.capabilitiesKhr.maxImageCount) {
            imageCount = details.capabilitiesKhr.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfoKhr = {};
        createInfoKhr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfoKhr.surface = surface;
        createInfoKhr.minImageCount = imageCount;
        createInfoKhr.imageFormat = surfaceFormatKhr.format;
        createInfoKhr.imageColorSpace = surfaceFormatKhr.colorSpace;
        createInfoKhr.imageExtent = extent2D;
        createInfoKhr.imageArrayLayers = 1;
        createInfoKhr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Queue::QueueFamilyIndices indices = Queue::findQueueFamilies(physicalDevice);
        uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily,
                                         (uint32_t) indices.presentFamily};
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfoKhr.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfoKhr.queueFamilyIndexCount = 2;
            createInfoKhr.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfoKhr.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfoKhr.queueFamilyIndexCount = 0;
            createInfoKhr.pQueueFamilyIndices = nullptr;
        }
        createInfoKhr.preTransform = details.capabilitiesKhr.currentTransform;
        createInfoKhr.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfoKhr.presentMode = presentModeKhr;
        createInfoKhr.clipped = VK_TRUE;
        createInfoKhr.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfoKhr, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain");
        }

        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
        swapChainImageFormat = surfaceFormatKhr.format;
        swapChainExtent = extent2D;
    }

    void Application::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.format = swapChainImageFormat;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.layerCount = 1;
            if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image view");
            }
        }
    }

    void Application::drawFrame() {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(),
                              imageAvailableSemaphores[currentFrame],
                              VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command");
        }

        VkPresentInfoKHR presentInfoKhr = {};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchainKhrs[] = {swapChain};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapchainKhrs;
        presentInfoKhr.pImageIndices = &imageIndex;
        presentInfoKhr.pResults = nullptr;
        vkQueuePresentKHR(graphicsQueue, &presentInfoKhr);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Application::update() {

    }
}