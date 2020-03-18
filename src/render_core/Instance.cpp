//
// Created by anuding on 2019/12/27.
//

#include <stdexcept>
#include "Instance.h"
#include "../utils/ValidationUtil.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine::RenderCore {
	Instance::Instance(bool enableValidationLayers) {
		createInstance(enableValidationLayers);
	}
	Instance::~Instance()
	{
		vkDestroyInstance(instance, nullptr);
	}
	VkInstance Instance::get()
	{
		return instance;
	}
	void Instance::printInfo()
	{
		uint32_t cnt;
		vkEnumeratePhysicalDevices(instance, &cnt, nullptr);
		if (cnt == 0)
			throw std::runtime_error("No physical device!");
		std::vector<VkPhysicalDevice> vkPhysicalDevices(cnt);
		vkEnumeratePhysicalDevices(instance, &cnt, vkPhysicalDevices.data());


		for (const auto& physicalDevice : vkPhysicalDevices) {
			VkPhysicalDeviceProperties pdp;
			vkGetPhysicalDeviceProperties(physicalDevice, &pdp);
			std::cout << pdp.deviceName << std::endl;

			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

			//for(auto &ext: availableExtensions)
			//	std::cout << "\t" << ext.extensionName << std::endl;
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
			for (auto& queueProperties : queueFamilyProperties)
			{
				if (queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					std::cout << "\t" << "VK_QUEUE_GRAPHICS";
				if (queueProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
					std::cout << "\t" << "VK_QUEUE_COMPUTE";
				if (queueProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
					std::cout << "\t" << "VK_QUEUE_TRANSFER";
				std::cout << "\n\t\t" << "COUNT: " << queueProperties.queueCount << std::endl;
			}
		}
	}
	void Instance::createInstance(bool enableValidationLayers) {
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


		auto requiredExtensions = getRequiredExtensions(enableValidationLayers);
		checkInstanceExtensionsSupport(requiredExtensions);
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoExt;
		if (enableValidationLayers) {
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Utils::validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = Utils::validationLayers.data();
			Utils::populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfoExt);
			instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugUtilsMessengerCreateInfoExt;
		}
		else {
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

	}

	std::vector<const char*> Instance::getRequiredExtensions(bool enableValidationLayers) {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = nullptr;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions,
			glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	void Instance::checkInstanceExtensionsSupport(const std::vector<const char*>& requiredExtensions) {
		uint32_t supportedExtensionsCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, nullptr);
		std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionsCount, supportedExtensions.data());

		for (int i = 0; i < requiredExtensions.size(); i++) {
			bool triggeer = false;
			for (auto& item : supportedExtensions)
				if (std::string(requiredExtensions[i]) == item.extensionName)
					triggeer = true;
			if (!triggeer)
				throw std::runtime_error("GLFW needs extension:" + std::string(requiredExtensions[i]));
		}
	}

	void Instance::checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> supportlayerProperties(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, supportlayerProperties.data());
		for (auto& layer : Utils::validationLayers) {
			bool trigger = false;
			for (auto& supportLayer : supportlayerProperties) {
				if (std::string(layer) == supportLayer.layerName)
					trigger = true;
			}
			if (!trigger)
				throw std::runtime_error("validation layers requested, but not available!");
		}
	}
}