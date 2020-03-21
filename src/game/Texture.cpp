#include "Texture.h"
#include "BufferHelper.h"
#include "VKContext.h"
using namespace Engine::RenderCore;
Texture::Texture(const char* path)
{
	Key key = assetManager.load(path);
	createTextureBuffer(key);
	imageView = createImageView();
}

Texture::~Texture()
{
}

void Texture::createTextureBuffer(Key key) {
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	auto asset = assetManager.getAssetByKey(key);

	auto metainfo = asset.getMetaInfo();
	BufferHelper::createBuffer(metainfo.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer);

	BufferHelper::allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		stagingBuffer, stagingMemory);
	void* data;
	vkMapMemory(device, stagingMemory, 0, metainfo.size, 0, &data);
	memcpy(data, asset.getData(), static_cast<size_t>(metainfo.size));
	vkUnmapMemory(device, stagingMemory);

	VkImageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent.width = static_cast<uint32_t>(metainfo.width);
	createInfo.extent.height = static_cast<uint32_t>(metainfo.height);
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT
		| VK_IMAGE_USAGE_SAMPLED_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.flags = 0;
	if (vkCreateImage(device, &createInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image");
	}
	BufferHelper::allocateImageMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);

	BufferHelper::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	BufferHelper::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(metainfo.width), static_cast<uint32_t>(metainfo.height));
	BufferHelper::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingMemory, nullptr);
}

VkImageView Texture::createImageView() {
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	createInfo.image = image;
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

	VkImageView imageView;
	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view");
	}
	return imageView;
}