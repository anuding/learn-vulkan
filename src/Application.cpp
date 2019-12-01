#include <vector>
#include <set>
#include "Application.h"
#include "Shader.h"

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
        createFramebuffers();
    }

    void Application::mainLoop() {
        while (!glfwWindowShouldClose(_window)) {
            glfwPollEvents();
        }
    }

    void Application::cleanUp() {

        for (auto framebuffer: _swapChainFramebuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
        vkDestroyRenderPass(_device, _renderPass, nullptr);
        for (auto imageView: _swapChainImageViews) {
            vkDestroyImageView(_device, imageView, nullptr);
        }
        vkDestroySwapchainKHR(_device, _swapChain, nullptr);
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
            SwapChainHelper::SwapChainSupportedDetails details = querySwapChainSupport(vkPhysicalDevice);
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


    SwapChainHelper::SwapChainSupportedDetails Application::querySwapChainSupport(VkPhysicalDevice vkPhysicalDevice) {
        SwapChainHelper::SwapChainSupportedDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, _surface, &details.capabilitiesKhr);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, _surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, _surface, &presentModeCount,
                                                      details.presentModes.data());
        }
        return details;
    }

    void Application::createSwapChain() {
        SwapChainHelper::SwapChainSupportedDetails details = querySwapChainSupport(_physicalDevice);
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
        auto vertShaderCode = Shader::readFile("../../spv_shaders/vert.spv");
        auto fragShaderCode = Shader::readFile("../../spv_shaders/frag.spv");
        VkShaderModule vertModule = Shader::createShaderModule(_device, vertShaderCode);
        VkShaderModule fragModule = Shader::createShaderModule(_device, fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageCreateInfo.module = vertModule;
        vertShaderStageCreateInfo.pName = "main";
        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageCreateInfo.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};
        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageCreateInfo.module = fragModule;
        fragShaderStageCreateInfo.pName = "main";
        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageCreateInfo.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageCreateInfo, fragShaderStageCreateInfo};

//        Fixed Functions
        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
        vertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
        vertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
        inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) _swapChainExtent.width;
        viewport.height = (float) _swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;


        VkRect2D scissor = {};
        scissor.extent = _swapChainExtent;
        scissor.offset = {0, 0};

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.pScissors = &scissor;
        viewportStateCreateInfo.pViewports = &viewport;
        viewportStateCreateInfo.scissorCount = 1;
        viewportStateCreateInfo.viewportCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCreateInfo.lineWidth = 1.0f;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f;
        multisampleStateCreateInfo.pSampleMask = nullptr;
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;


        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};

        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
        colorBlendAttachmentState.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
        colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
        colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(_device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipelinelayout");
        }

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.renderPass = _renderPass;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
        pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
        pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.pDynamicState = nullptr;
        pipelineCreateInfo.layout = _pipelineLayout;
        pipelineCreateInfo.renderPass = _renderPass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &_graphicsPipeline) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline");
        }

        vkDestroyShaderModule(_device, vertModule, nullptr);
        vkDestroyShaderModule(_device, fragModule, nullptr);


    }

    void Application::createRenderPass() {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = _swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;

        if (vkCreateRenderPass(_device, &renderPassCreateInfo, nullptr, &_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass");
        }
    }

    void Application::createFramebuffers() {
        _swapChainFramebuffers.resize(_swapChainImageViews.size());
        for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                    _swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = _renderPass;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.width = _swapChainExtent.width;
            framebufferCreateInfo.height = _swapChainExtent.height;
            framebufferCreateInfo.layers = 1;

            if (vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &_swapChainFramebuffers[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer");
            }
        }
    }

}