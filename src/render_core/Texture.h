#pragma once
#include <vulkan/vulkan.h>
#include "Image.h"
#include "Buffer.h"

namespace Engine::RenderCore {
	class Application;
	class Texture {
	public:
		Texture() = default;
		~Texture();
		void init(Application* app,  char* path);

	private:
		char* path;
		Application* app;
		Image image;
		Buffer stagingBuffer;
		void createTexture();
		void loadRawImage(const char* path, unsigned char*& imageData, uint64_t& dataSize);

	};
}