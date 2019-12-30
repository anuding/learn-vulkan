//
// Created by anuding on 2019/12/27.
//

#include <stdexcept>
#include "Descriptor.h"
#include "Shader.h"

namespace Engine::RenderCore::DescriptorHelper {

    void createDescriptorLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 1;
        descriptorSetLayoutCreateInfo.pBindings = &uboLayoutBinding;
        if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout");
        }
    }

    void createDescriptorPool() {
        VkDescriptorPoolSize poolSize = {};
        poolSize.descriptorCount = static_cast<uint32_t >(swapChainImages.size());
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

        VkDescriptorPoolCreateInfo poolCreateInfo = {};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.poolSizeCount = 1;
        poolCreateInfo.pPoolSizes = &poolSize;
        poolCreateInfo.maxSets = static_cast<uint32_t > (swapChainImages.size());
        if (vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool");
        }

    }



    void createDescriptorSets() {
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkDescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(ShaderHelper::UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
        }
    }

    void allocateDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo setAllocateInfo = {};
        setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        setAllocateInfo.descriptorPool = descriptorPool;
        setAllocateInfo.pSetLayouts = layouts.data();
        setAllocateInfo.descriptorSetCount = static_cast<uint32_t > (swapChainImages.size());

        descriptorSets.resize(swapChainImages.size());
        if (vkAllocateDescriptorSets(device, &setAllocateInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets");
        }
    }

    void init() {
        createDescriptorLayout();
        createDescriptorPool();
        allocateDescriptorSets();
    }

}
