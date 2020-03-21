//
// Created by anuding on 2019/12/24.
//

#include <stdexcept>
#include <set>
#include <vector>
#include "Device.h"
#include "Application.h"

namespace Engine::RenderCore {
	Device::~Device() {
		vkDestroyDevice(this->get(), nullptr);
	}

	void Device::init(Application* app)
	{
		this->app = app;
		createLogicalDevice(app->physicalDevice.get());
	}

	void Device::createLogicalDevice(VkPhysicalDevice physicalDevice) {
		auto availableQueueFamilies = Utils::getQueueFamilies(physicalDevice, app->surface.get());
		for (auto& f : availableQueueFamilies) {
			if (f.hasAbility(Utils::GRAPHICS)
				&& f.hasAbility(Utils::COMPUTE)
				&& f.hasAbility(Utils::PRESENT)
				&& f.hasAbility(Utils::TRANSFER)
				&& f.hasAbility(Utils::SPARSE))
			{
				universalQueueFamilyIndex = f.index;
				break;
			}
		}
		if(universalQueueFamilyIndex == -1)
			throw std::runtime_error("failed to find an universal queueFamily!");

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float queuePriority[3] = { 1.0f , 1.0f, 1.0f};
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = universalQueueFamilyIndex;
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

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(this->get(), universalQueueFamilyIndex, 0, &graphicsQueue);
		vkGetDeviceQueue(this->get(), universalQueueFamilyIndex, 1, &transferQueue);
		vkGetDeviceQueue(this->get(), universalQueueFamilyIndex, 2, &computeQueue);
	}

}