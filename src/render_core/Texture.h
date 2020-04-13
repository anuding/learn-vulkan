#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Engine::RenderCore {
	class Application;

	class Texture {
	public:
		Texture() = default;
		~Texture();
		void init(Application* app);

	private:
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
		void createTexture();
	};
}