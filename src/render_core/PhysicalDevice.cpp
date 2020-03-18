#include "PhysicalDevice.h"
#include "../utils/DeviceQueueUtil.h"
#include <stdexcept>
#include <vector>
#include <set>
namespace Engine::RenderCore {
	PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
	{
		surfaceCopy = surface;
		pickPhysicalDevice(instance);
	}

	VkPhysicalDevice PhysicalDevice::get() {
		return this->physicalDevice;
	}


	void PhysicalDevice::pickPhysicalDevice(VkInstance instance) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
			throw std::runtime_error("No physical device!");
		std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, vkPhysicalDevices.data());

		uint32_t topRank = 0;
		for (const auto& pdevice : vkPhysicalDevices) {
			if (!isPhysicalDeviceSuitable(pdevice))
				continue;
			uint32_t rank = 0;
			std::vector<Utils::QueueFamily> queueFamilies = Utils::getQueueFamilies(pdevice, surfaceCopy);
			for (auto& qf : queueFamilies) {
				rank += qf.count;
			}
			if (topRank < rank) {
				topRank = rank;
				physicalDevice = pdevice;
				Utils::selectedQueueFamilies = queueFamilies;
			}
		}
		if (physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("These devices are not suitable!");
	}

	bool PhysicalDevice::isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice) {
		bool isExtensionsSupported = checkPhysicalDeviceExtensionSupport(physicalDevice);

		bool swapChainAdequate = false;
		if (isExtensionsSupported) {
			Utils::SwapChainSupportedDetails details = Utils::querySwapChainSupport(physicalDevice, surfaceCopy);
			swapChainAdequate = !details.formats.empty()
				&& !details.presentModes.empty();
		}
		return isExtensionsSupported && swapChainAdequate;
	}

	bool PhysicalDevice::checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(Utils::physicalDeviceExtensions.begin(), Utils::physicalDeviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		return requiredExtensions.empty();
	}
}