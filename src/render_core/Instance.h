//
// Created by anuding on 2019/12/27.
//

#ifndef LEARN_VULKAN_INSTANCE_H
#define LEARN_VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine::RenderCore {
	class Instance
	{
	public:
		Instance(bool enableValidationLayers);
		~Instance();
		VkInstance get();
		void printInfo();
	private:
		VkInstance instance;

		void createInstance(bool enableValidationLayers);

		std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

		void checkInstanceExtensionsSupport(const std::vector<const char*>& requiredExtensions);

		void checkValidationLayerSupport();
	};
}

#endif //LEARN_VULKAN_INSTANCE_H
