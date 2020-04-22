#pragma once
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Buffer.h"
#include "BufferFactory.h"
#include "ImageFactory.h"
#include <stdexcept>
#include "Application.h"

namespace Engine::RenderCore {
	Texture::~Texture() {

	}
	void Texture::init(Application* app, char* path)
	{
		this->app = app;
		this->path = path;
		createTexture();
	}
	void Texture::createTexture()
	{
		//Buffer side
		unsigned char* data;
		int width, height;
		loadRawImage(path, data, width, height);
		uint64_t dataSize = width * height * 4;
		auto stagingBuffer = BufferFactory::createStagingBuffer(this->app, dataSize);
		stagingBuffer->updateBufferMemory(data, dataSize);
		stbi_image_free(data);

		//Image side
		image = ImageFactory::createImage(this->app, width, height);
		image->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		//Copy buffer to image
		VkCommandBuffer commandBuffer = this->app->commandPool.beginSingleTimeCommands();
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
		this->app->commandPool.endSingleTimeCommands(commandBuffer);


		image->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	}

	void Texture::loadRawImage(const char* path, unsigned char*& imageData, int& width, int& height)
	{
		int channels;
		imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

		if (!imageData)
			throw std::runtime_error("failed to load raw image from");
	}
}