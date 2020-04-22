#pragma once
#include "Sampler.h"
#include "Application.h"

namespace Engine::RenderCore {
	Sampler::~Sampler()
	{
		vkDestroySampler(app->device.get(), this->get(), nullptr);
	}
	void Sampler::init(Application* app)
	{
		this->app = app;
		createSampler();
	}
	void Sampler::createSampler()
	{
		VkSamplerCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.anisotropyEnable = VK_TRUE;
		createInfo.maxAnisotropy = 16;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(app->device.get(), &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create sampler");
		}
	}
}