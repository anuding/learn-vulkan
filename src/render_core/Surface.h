#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "VkComp.h"

namespace Engine::RenderCore {
	class Surface: public VkComp<VkSurfaceKHR> {
	public:
		Surface() = default;
		~Surface();
		void init(Application* app) override;

	private:

		void createSurface();
	};
}