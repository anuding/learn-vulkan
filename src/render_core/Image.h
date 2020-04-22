#pragma once
#include <vulkan/vulkan.h>
#include "VKComp.h"

namespace Engine::RenderCore {
	class Image : public VkComp<VkImage> {
	public:
		Image() = default;
		~Image();

		VkImageView imageView;

		virtual void init(Application* app) override;
		void createImage(const VkImageCreateInfo& createInfo, const VkMemoryPropertyFlags& properties);
		void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	private:
		VkDeviceMemory imageMemory;

		uint32_t findImageMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	};
}