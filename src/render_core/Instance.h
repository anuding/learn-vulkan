//
// Created by anuding on 2019/12/27.
//

#ifndef LEARN_VULKAN_INSTANCE_H
#define LEARN_VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <vector>
#include "VkComp.h"
namespace Engine::RenderCore {
	class Instance : public VkComp<VkInstance>
	{
	public:
		Instance() = default;
		~Instance();
		void init(Application* app) override;
		void printInfo();
	private:

		void createInstance(bool enableValidationLayers);

		std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

		void checkInstanceExtensionsSupport(const std::vector<const char*>& requiredExtensions);

		void checkValidationLayerSupport();
	};
}

#endif //LEARN_VULKAN_INSTANCE_H
