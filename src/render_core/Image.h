#pragma once
#include <vulkan/vulkan.h>
#include "VKComp.h"

namespace Engine::RenderCore {
	class Image : public VkComp<VkImage> {
	public:
		Image() = default;
		~Image();

		virtual void init(Application* app) override;
		void createImage(const VkImageCreateInfo& createInfo, const VkMemoryPropertyFlags& properties);
		//void updateImageMemory(unsigned char* data, uint32_t size);
		void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	private:
		VkImageView imageView;
		VkDeviceMemory imageMemory;

		uint32_t findImageMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	};
}