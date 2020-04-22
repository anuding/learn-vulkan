#pragma once
#include <vulkan/vulkan.h>
#include "Image.h"
#include <memory>

namespace Engine::RenderCore {
	class Application;
	class Texture {
	public:
		Texture() = default;
		~Texture();
		void init(Application* app, char* path);

	private:
		char* path;
		Application* app;
		std::unique_ptr<Image> image;
		void createTexture();
		void loadRawImage(const char* path, unsigned char*& imageData, int& width, int& height);

	};
}