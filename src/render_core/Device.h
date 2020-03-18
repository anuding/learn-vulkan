//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_DEVICE_H
#define ENGINE_DEVICE_H

#include <vulkan/vulkan.h>
#include "../utils/ValidationUtil.h"
#include "../utils/DeviceQueueUtil.h"
namespace Engine::RenderCore {

	class Device
	{
	public:
		Device(VkPhysicalDevice physicalDevice);
		~Device();
		VkDevice get();
	private:
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue transferQueue;
		VkQueue computeQueue;

		void createLogicalDevice(VkPhysicalDevice physicalDevice);
	};
}


#endif //ENGINE_DEVICE_H
