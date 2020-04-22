#include "Window.h"
#include "Application.h"

namespace Engine::RenderCore {
	void Window::init(Application* app)
	{
		this->app = app;
		if(!glfwInit())
			throw std::runtime_error("failed to init GLFW!");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
		this->get() = glfwCreateWindow(this->app->WINDOW_WIDTH, this->app->WINDOW_HEIGHT, this->app->WINDOW_NAME, nullptr, nullptr);
		if(!this->get())
			throw std::runtime_error("failed to create window!");
	}

	Window::~Window()
	{
		if (this->app == nullptr)
			return;
		glfwDestroyWindow(this->app->window.get());
		glfwTerminate();
	}
}