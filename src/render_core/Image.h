#pragma once
#include <vulkan/vulkan.h>
#include "VKComp.h"

namespace Engine::RenderCore {
	class Image : public VkComp<VkImage> {
	public:
		Image() = default;
		~Image();

		virtual void init(Application* app) override;
		VkImageCreateInfo createInfo = {};

	private:
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};
}