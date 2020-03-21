//
// Created by anuding on 2019/12/24.
//

#include <iostream>
#include "ValidationUtil.h"

namespace Engine::Utils {
    bool enableValidationLayers = false;
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage <<
            std::endl;
        return VK_FALSE;
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
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


    ValidationLayerDebugger::ValidationLayerDebugger(VkInstance instance)
    {
        enableValidationLayers = true;
        this->instanceCopy = instance;
        this->setupDebugMessenger();
    }

    ValidationLayerDebugger::~ValidationLayerDebugger() {
        this->DestroyDebugUtilsMessengerEXT();
    }

    void ValidationLayerDebugger::setupDebugMessenger() {
        if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfoExt;
        populateDebugMessengerCreateInfo(createInfoExt);
        if (CreateDebugUtilsMessengerEXT(&createInfoExt, nullptr, &this->debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    VkResult ValidationLayerDebugger::CreateDebugUtilsMessengerEXT(
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(this->instanceCopy,
                                      "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(this->instanceCopy, pCreateInfo, pAllocator,
                        pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void ValidationLayerDebugger::DestroyDebugUtilsMessengerEXT() {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(this->instanceCopy,
                                      "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(this->instanceCopy, this->debugMessenger, nullptr);
        }
    }

}

