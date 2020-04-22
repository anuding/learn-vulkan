#pragma once
//
// Created by anuding on 2019/12/27.
//
#include <vulkan/vulkan.h>
#include <vector>
#include "VkComp.h"
namespace Engine::RenderCore{
    class DescriptorPool : public VkComp<VkDescriptorPool> {
    public:
        DescriptorPool() = default;
        ~DescriptorPool();
        virtual void init(Application* app) override;

        std::vector<VkDescriptorSet> sets;
        VkDescriptorSetLayout layout;

    private:

        void createDescriptorPool();
        
        void createDescriptorLayout();

        void createDescriptorSets();
    };
}