#include "ImageFactory.h"
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>
#include "BufferFactory.h"
#include "Application.h"

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


    std::unique_ptr<Image> createImageFromFile(Application* app, const char* path)
    {

		int width, height, channels;
		unsigned char* imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
		if (!imageData)
			throw std::runtime_error("failed to load raw image from");

		//Buffer side
		uint64_t dataSize = width * height * 4;
		auto stagingBuffer = BufferFactory::createStagingBuffer(app, dataSize);
		stagingBuffer->updateBufferMemory(imageData, dataSize);
		stbi_image_free(imageData);

		//Image side
		std::unique_ptr<Image> image = ImageFactory::createImage(app, width, height);
		image->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		//Copy buffer to image
		VkCommandBuffer commandBuffer = app->commandPool.beginSingleTimeCommands();
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer->get(), image->get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		app->commandPool.endSingleTimeCommands(commandBuffer);


		image->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		return image;
    }
}
