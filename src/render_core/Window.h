#pragma once
#include <GLFW/glfw3.h>
#include "VKComp.h"

namespace Engine::RenderCore {
	class Window : public VkComp<GLFWwindow*> {
	public:
		Window() = default;
		~Window();

		void init(Application* app) override;
	};
}