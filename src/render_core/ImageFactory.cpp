#include "ImageFactory.h"
#include "Image.h"

namespace Engine::RenderCore::ImageFactory {
	std::unique_ptr<Image> createImage(Application* app, uint32_t width, uint32_t height)
	{
		std::unique_ptr<Image> image(new Image());
		VkImageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.extent.width = width;
        createInfo.extent.height = height;
        createInfo.extent.depth = 1;
        createInfo.mipLevels = 1;
        createInfo.arrayLayers = 1;
        createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		image->init(app);
		image->createImage(createInfo, properties);
		return image;
	}
}
