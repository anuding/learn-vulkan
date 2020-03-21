#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::Utils {
	struct SwapChainSupportedDetails {
		VkSurfaceCapabilitiesKHR capabilitiesKhr;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};
	enum QueueFamilyAbility {
		GRAPHICS, COMPUTE, PRESENT, TRANSFER, SPARSE
	};
	struct QueueFamily {
		uint32_t index = -1;
		uint32_t count = 0;
		std::vector<QueueFamilyAbility> abilities;

		bool hasAbility(QueueFamilyAbility a) {
			for (auto& _a : abilities) {
				if (_a == a)
					return true;
			}
			return false;
		}
	};

	extern const std::vector<const char*> physicalDeviceExtensions;
	extern std::vector<QueueFamily> getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	extern SwapChainSupportedDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	//extern getQueue();
}