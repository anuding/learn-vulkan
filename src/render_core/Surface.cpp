#include "Surface.h"
#include <stdexcept>
namespace Engine::RenderCore {
	Surface::Surface(VkInstance instance, GLFWwindow* window)
	{
		this->instanceCopy = instance;
		this->windowCopy = window;
		createSurface();
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(instanceCopy, surface, nullptr);
	}

	VkSurfaceKHR Surface::get()
	{
		return surface;
	}

	void Surface::createSurface()
	{
		if (glfwCreateWindowSurface(instanceCopy, windowCopy, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
}
