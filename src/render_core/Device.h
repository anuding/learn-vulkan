//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_DEVICE_H
#define ENGINE_DEVICE_H

#include <vulkan/vulkan.h>
#include "../utils/ValidationUtil.h"
#include "../utils/DeviceQueueUtil.h"
#include "VkComp.h"
namespace Engine::RenderCore {

	class Device : public VkComp<VkDevice> 
	{
	public:
		Device() = default;
		~Device();
		void init(Application* app) override;

		uint32_t universalQueueFamilyIndex = -1;
		VkQueue graphicsQueue;
		VkQueue transferQueue;
		VkQueue computeQueue;
	private:


		void createLogicalDevice(VkPhysicalDevice physicalDevice);
	};
}


#endif //ENGINE_DEVICE_H
