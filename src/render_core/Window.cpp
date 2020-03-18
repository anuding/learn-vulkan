#include "Window.h"

namespace Engine::RenderCore {
	Window::Window(uint32_t width, uint32_t height, const char* windowName)
		:WIDTH(width), HEIGHT(height), WINDOW_NAME(windowName)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
		this->window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);
	}
	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	GLFWwindow* Window::get()
	{
		return window;
	}
}