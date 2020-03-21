#pragma once
#include <vulkan/vulkan.h>
#include "VkComp.h"
namespace Engine::RenderCore {
	class PhysicalDevice : public VkComp<VkPhysicalDevice>
	{
	public:
		PhysicalDevice() = default;
		
		// Inherited via VkComp
		virtual void init(Application* app) override;
	private:

		void pickPhysicalDevice(VkInstance instance);

		bool isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);

		bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

	};

}
