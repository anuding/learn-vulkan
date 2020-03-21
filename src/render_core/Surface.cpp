#include "Surface.h"
#include "Application.h"
#include <stdexcept>
namespace Engine::RenderCore {
	void Surface::init(Application* app)
	{
		this->app = app;
		createSurface();
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(app->instance.get(), app->surface.get(), nullptr);
	}



	void Surface::createSurface()
	{
		if (glfwCreateWindowSurface(app->instance.get(), app->window.get(), nullptr, &app->surface.get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
}
