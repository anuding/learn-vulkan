#pragma once
#include "Image.h"
#include <stdexcept>
#include "Application.h"
namespace Engine::RenderCore {
	Image::~Image()
	{
		vkDestroyImage(this->app->device.get(), this->get(), nullptr);
	}
	void Image::init(Application* app)
	{
		this->app = app;
		if (vkCreateImage(this->app->device.get(), &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create Image!");
		}
	}
}