#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "Image.h"
#include <memory>

namespace Engine::RenderCore::ImageFactory {
	std::unique_ptr<Image> createImage(Application* app, uint32_t width, uint32_t height);
}
