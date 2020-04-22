#pragma once
//
// Created by anuding on 2019/12/27.
//

#include <stdexcept>
#include "DescriptorPool.h"
#include "Shader.h"
#include "Application.h"
#include <array>
namespace Engine::RenderCore {

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(this->app->device.get(), this->get(), nullptr);
		vkDestroyDescriptorSetLayout(this->app->device.get(), layout, nullptr);
		vkDestroyDescriptorSetLayout(this->app->device.get(), layout, nullptr);
	}

	void DescriptorPool::init(Application* app)
	{
		this->app = app;
		createDescriptorPool();

		createDescriptorLayout();
		createDescriptorSets();
	}


	void DescriptorPool::createDescriptorPool() {
		std::array<VkDescriptorPoolSize, 2> poolSizes = {};
		poolSizes[0].descriptorCount = static_cast<uint32_t>(this->app->swapChain.imageCount);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(this->app->swapChain.imageCount);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		VkDescriptorPoolCreateInfo poolCreateInfo = {};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolCreateInfo.pPoolSizes = poolSizes.data();
		poolCreateInfo.maxSets = static_cast<uint32_t> (this->app->swapChain.imageCount);
		if (vkCreateDescriptorPool(this->app->device.get(), &poolCreateInfo, nullptr, &this->get()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool");
		}

	}

	void DescriptorPool::createDescriptorLayout() {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding,samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();
		if (vkCreateDescriptorSetLayout(this->app->device.get(), &descriptorSetLayoutCreateInfo, nullptr, &layout) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout");
		}
	}

	void DescriptorPool::createDescriptorSets() {
		std::vector<VkDescriptorSetLayout> layouts(this->app->swapChain.imageCount, layout);
		VkDescriptorSetAllocateInfo setAllocateInfo = {};
		setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		setAllocateInfo.descriptorPool = this->get();
		setAllocateInfo.pSetLayouts = layouts.data();
		setAllocateInfo.descriptorSetCount = static_cast<uint32_t> (this->app->swapChain.imageCount);

		sets.resize(this->app->swapChain.imageCount);
		if (vkAllocateDescriptorSets(this->app->device.get(), &setAllocateInfo, sets.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets");
		}

		for (size_t i = 0; i < this->app->swapChain.imageCount; i++) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = this->app->uniformBuffers[i]->get();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UBO_MVP);

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = this->app->tex->imageView;
			imageInfo.sampler = this->app->sampler.get();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = sets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = sets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;
			vkUpdateDescriptorSets(this->app->device.get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
}
