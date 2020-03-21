//
// Created by anuding on 2019/11/21.
//

#include <limits>
#include <algorithm>
#include <stdexcept>
#include "SwapChain.h"
#include "Application.h"
#include "../utils/DeviceQueueUtil.h"

namespace Engine::RenderCore {
	SwapChain::~SwapChain()
	{
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(app->device.get(), imageView, nullptr);
		}
		vkDestroySwapchainKHR(app->device.get(), this->get(), nullptr);
	}
	void SwapChain::init(Application* app)
	{
		this->app = app;
		createSwapChain(app->WINDOW_WIDTH, app->WINDOW_HEIGHT);
		createSwapChainImages();
		createSwapChainImageViews();
	}
	void SwapChain::createSwapChain(uint32_t width, uint32_t height) {
		Utils::SwapChainSupportedDetails details = Utils::querySwapChainSupport(app->physicalDevice.get(), app->surface.get());
		VkSurfaceFormatKHR surfaceFormatKhr = chooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentModeKhr = chooseSwapPresentMode(details.presentModes);
		VkExtent2D extent2D = chooseSwapExtent(details.capabilitiesKhr, width, height);

		imageCount = details.capabilitiesKhr.minImageCount + 1;
		if (details.capabilitiesKhr.maxImageCount > 0
			&& imageCount > details.capabilitiesKhr.maxImageCount) {
			imageCount = details.capabilitiesKhr.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfoKhr = {};
		createInfoKhr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfoKhr.surface = app->surface.get();
		createInfoKhr.minImageCount = imageCount;
		createInfoKhr.imageFormat = surfaceFormatKhr.format;
		createInfoKhr.imageColorSpace = surfaceFormatKhr.colorSpace;
		createInfoKhr.imageExtent = extent2D;
		createInfoKhr.imageArrayLayers = 1;
		createInfoKhr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfoKhr.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfoKhr.queueFamilyIndexCount = 0;
		createInfoKhr.pQueueFamilyIndices = nullptr;
		createInfoKhr.preTransform = details.capabilitiesKhr.currentTransform;
		createInfoKhr.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfoKhr.presentMode = presentModeKhr;
		createInfoKhr.clipped = VK_TRUE;
		createInfoKhr.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(app->device.get(), &createInfoKhr, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain");
		}


		this->swapChainImageFormat = surfaceFormatKhr.format;
		this->swapChainExtent = extent2D;
	}

	void SwapChain::createSwapChainImages()
	{
		vkGetSwapchainImagesKHR(app->device.get(), this->get(), &imageCount, nullptr);
		this->swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(app->device.get(), this->get(), &imageCount, this->swapChainImages.data());
	}

	void SwapChain::createSwapChainImageViews() {
		swapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.format = swapChainImageFormat;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(app->device.get(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image view");
			}
		}
	}


	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
			return { VK_FORMAT_B8G8R8A8_UNORM,
					VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		for (auto& format : availableFormats) {
			if (format.format == VK_FORMAT_R8G8B8_UNORM
				&& format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
				return format;
		}
		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
		for (auto& mode : availablePresentModes) {
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return mode;
			else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				bestMode = mode;
		}
		return bestMode;
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { width, height };
			actualExtent.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	}
}
