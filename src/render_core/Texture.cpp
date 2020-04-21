#pragma once
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Buffer.h"
#include "BufferFactory.h"
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
		uint64_t dataSize;
		loadRawImage(path, data, dataSize);
		auto stagingBuffer = BufferFactory::createStagingBuffer(this->app, dataSize);
		stagingBuffer->updateBufferMemory(data, dataSize);

		//Image side
	}
	
	void Texture::loadRawImage(const char* path, unsigned char* & imageData, uint64_t& dataSize)
	{
		int width, height, channels;
		imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
		dataSize = width * height * 4;
		if (!imageData)
			throw std::runtime_error("failed to load raw image from");
	}
}