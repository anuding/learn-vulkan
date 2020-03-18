//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include <set>
#include <vector>
#include "Device.h"


namespace Engine::RenderCore {
	Device::Device(VkPhysicalDevice physicalDevice) {
		createLogicalDevice(physicalDevice);
	}

	Device::~Device() {
		vkDestroyDevice(device, nullptr);
	}

	VkDevice Device::get()
	{
		return VkDevice();
	}

	void Device::createLogicalDevice(VkPhysicalDevice physicalDevice) {
		uint32_t universalFamilyIndex = -1;
		for (auto& f : Utils::selectedQueueFamilies) {
			if (f.hasAbility(Utils::GRAPHICS)
				&& f.hasAbility(Utils::COMPUTE)
				&& f.hasAbility(Utils::PRESENT)
				&& f.hasAbility(Utils::TRANSFER)
				&& f.hasAbility(Utils::SPARSE))
				universalFamilyIndex = f.index;
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriority[3] = { 1.0f , 1.0f, 1.0f};
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = universalFamilyIndex;
		queueCreateInfo.queueCount = 3;
		queueCreateInfo.pQueuePriorities = queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);

		VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = {};
		vkPhysicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &vkPhysicalDeviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(Utils::physicalDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = Utils::physicalDeviceExtensions.data();
		if (Utils::enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t> (Utils::validationLayers.size());
			createInfo.ppEnabledLayerNames = Utils::validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(device, universalFamilyIndex, 0, &graphicsQueue);
		vkGetDeviceQueue(device, universalFamilyIndex, 1, &transferQueue);
		vkGetDeviceQueue(device, universalFamilyIndex, 2, &computeQueue);
	}

}