//
// Created by anuding on 2019/12/24.
//

#include <iostream>
#include "ValidationLayer.h"
#include "VKContext.h"

namespace Engine::RenderCore::DebugUtils {

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage <<
                  std::endl;
        return VK_FALSE;
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
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

    void setupDebugMessenger(bool enableValidationLayers) {
        if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfoExt;
        populateDebugMessengerCreateInfo(createInfoExt);
        if (CreateDebugUtilsMessengerEXT(&createInfoExt, nullptr, &RenderCore::debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    VkResult CreateDebugUtilsMessengerEXT(
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(RenderCore::instance,
                                      "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(RenderCore::instance, pCreateInfo, pAllocator,
                        pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT() {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
                vkGetInstanceProcAddr(RenderCore::instance,
                                      "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(RenderCore::instance, debugMessenger, nullptr);
        }
    }

}