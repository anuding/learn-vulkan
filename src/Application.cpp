#include <vector>
#include <set>
#include "Application.h"
#include "Shader.h"
#include "Semaphore.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"


namespace Engine::RenderCore {

    void Application::run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }


    void Application::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
        _window = glfwCreateWindow(WIDTH, HEIGHT, "blacklist4000", nullptr, nullptr);
    }

    void Application::initVulkan() {
        createInstance();
        setupDebugMessenger();

        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipelines();
        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObjects();
    }

    void Application::mainLoop() {
        while (!glfwWindowShouldClose(_window)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(_device);
    }

    void Application::cleanUp() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(_device, _inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(_device, _commandPool, nullptr);
        for (auto framebuffer: _swapChainFrameBuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
        vkDestroyRenderPass(_device, _renderPass, nullptr);
        for (auto imageView: _swapChainImageViews) {
            vkDestroyImageView(_device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(_device, _swapChain, nullptr);
        //this is a red line
        vkDestroyDevice(_device, nullptr);
        if (enableValidationLayers)
            DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyInstance(_instance, nullptr);
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void Application::createInstance() {
        if (enableValidationLayers)
            checkValidationLayerSupport();
        VkInstanceCreateInfo instanceCreateInfo = {};
        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = "blacklist4000";
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
            populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfoExt);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugUtilsMessengerCreateInfoExt;

        } else {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create _instance!");
        }

    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage <<
                  std::endl;
        return VK_FALSE;
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

    void Application::populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType =
                VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void Application::setupDebugMessenger() {
        if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfoExt;
        populateDebugMessengerCreateInfo(createInfoExt);
        if (CreateDebugUtilsMessengerEXT(_instance, &createInfoExt, nullptr,
                                         &_debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    VkResult Application::CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(instance,
                                      "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator,
                        pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void Application::DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                    VkDebugUtilsMessengerEXT debugMessenger, const
                                                    VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(instance,
                                      "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void Application::pickPhysicalDevice() {
        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("No physical _device!");
        std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(_instance, &deviceCount, vkPhysicalDevices.data());

        for (const auto &device: vkPhysicalDevices) {
            if (isDeviceSuitable(device))
                _physicalDevice = device;
        }
        if (_physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("These devices are not suitable!");
    }

    void Application::checkPhysicalDeviceSupport(std::vector<VkPhysicalDevice> &vkPhysicalDevices) {
        for (const auto &device: vkPhysicalDevices) {
            if (isDeviceSuitable(device))
                _physicalDevice = device;
        }
        if (_physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("These devices are not suitable!");
    }

    bool Application::isDeviceSuitable(VkPhysicalDevice vkPhysicalDevice) {
        QueueFamilyIndices indices = findQueueFamilies(vkPhysicalDevice);
        bool isExtensionsSupported = checkPhysicalDeviceExtensionSupport(vkPhysicalDevice);

        bool swapChainAdequate = false;
        if (isExtensionsSupported) {
            SwapChainHelper::SwapChainSupportedDetails details = SwapChainHelper::querySwapChainSupport(
                    vkPhysicalDevice, _surface);
            swapChainAdequate = !details.formats.empty()
                                && !details.presentModes.empty();
        }
        return findQueueFamilies(vkPhysicalDevice).isComplete()
               && isExtensionsSupported
               && swapChainAdequate;
    }

    QueueFamilyIndices Application::findQueueFamilies(VkPhysicalDevice vkPhysicalDevice) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        VkBool32 presentSupport = false;
        int i = 0;
        for (auto &qfp: queueFamilyProperties) {
            if (qfp.queueCount > 0 && qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, _surface, &presentSupport);
            if (qfp.queueCount > 0 && presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
            i++;
        }
        return indices;
    }

    void Application::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);
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

        if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical _device!");
        }
        vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);
        vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);
    }

    void Application::createSurface() {
        if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create _window _surface!");
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
        SwapChainHelper::SwapChainSupportedDetails details = SwapChainHelper::querySwapChainSupport(_physicalDevice,
                                                                                                    _surface);
        VkSurfaceFormatKHR surfaceFormatKhr = SwapChainHelper::chooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentModeKhr = SwapChainHelper::chooseSwapPresentMode(details.presentModes);
        VkExtent2D extent2D = SwapChainHelper::chooseSwapExtent(details.capabilitiesKhr);

        uint32_t imageCount = details.capabilitiesKhr.minImageCount + 1;
        if (details.capabilitiesKhr.maxImageCount > 0
            && imageCount > details.capabilitiesKhr.maxImageCount) {
            imageCount = details.capabilitiesKhr.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfoKhr = {};
        createInfoKhr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfoKhr.surface = _surface;
        createInfoKhr.minImageCount = imageCount;
        createInfoKhr.imageFormat = surfaceFormatKhr.format;
        createInfoKhr.imageColorSpace = surfaceFormatKhr.colorSpace;
        createInfoKhr.imageExtent = extent2D;
        createInfoKhr.imageArrayLayers = 1;
        createInfoKhr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);
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

        if (vkCreateSwapchainKHR(_device, &createInfoKhr, nullptr, &_swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain");
        }

        vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
        _swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());
        _swapChainImageFormat = surfaceFormatKhr.format;
        _swapChainExtent = extent2D;
    }

    void Application::createImageViews() {
        _swapChainImageViews.resize(_swapChainImages.size());
        for (size_t i = 0; i < _swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.format = _swapChainImageFormat;
            createInfo.image = _swapChainImages[i];
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
            if (vkCreateImageView(_device, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image view");
            }
        }
    }

    void Application::createGraphicsPipelines() {
        PipelineHelper::createGraphicsPipelines(_device, _swapChainExtent, _pipelineLayout, _renderPass,
                                                _graphicsPipeline);
    }

    void Application::createFrameBuffers() {
        FrameBufferHelper::createFrameBuffers(_device, _swapChainFrameBuffers, _swapChainImageViews, _renderPass,
                                              _swapChainExtent);
    }

    void Application::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice);
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        commandPoolCreateInfo.flags = 0;

        if (vkCreateCommandPool(_device, &commandPoolCreateInfo, nullptr, &_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool");
        }
    }

    void Application::createCommandBuffers() {
        _commandBuffers.resize(_swapChainFrameBuffers.size());
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool = _commandPool;
        commandBufferAllocateInfo.commandBufferCount = (uint32_t) _commandBuffers.size();
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        if (vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, _commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command buffers");
        }

        for (size_t i = 0; i < _commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            VkRenderPassBeginInfo renderPassBeginInfo = {};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = _renderPass;
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.framebuffer = _swapChainFrameBuffers[i];
            renderPassBeginInfo.renderArea.offset = {0, 0};
            renderPassBeginInfo.renderArea.extent = _swapChainExtent;
            VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassBeginInfo.pClearValues = &clearValue;


            if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin");
            }
            vkCmdBeginRenderPass(_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
            vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(_commandBuffers[i]);
            if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer");
            }
        }
    }

    void Application::drawFrame() {
        vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(_device, _swapChain, std::numeric_limits<uint64_t>::max(),
                              _imageAvailableSemaphores[_currentFrame],
                              VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command");
        }

        VkPresentInfoKHR presentInfoKhr = {};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchainKhrs[] = {_swapChain};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapchainKhrs;
        presentInfoKhr.pImageIndices = &imageIndex;
        presentInfoKhr.pResults = nullptr;
        vkQueuePresentKHR(_graphicsQueue, &presentInfoKhr);
        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Application::createRenderPass() {
        RenderPassHelper::createRenderPass(_device, _swapChainImageFormat, _renderPass);
    }

    void Application::createSyncObjects() {
        SemaphoreHelper::createSyncObjects(_device, _imageAvailableSemaphores, _renderFinishedSemaphores,
                                           _inFlightFences, MAX_FRAMES_IN_FLIGHT);
    }


}