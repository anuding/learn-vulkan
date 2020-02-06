//
// Created by anuding on 2019/12/27.
//

#ifndef LEARN_VULKAN_DESCRIPTOR_H
#define LEARN_VULKAN_DESCRIPTOR_H

#include "VKContext.h"
namespace Engine::RenderCore::DescriptorHelper{
    void init();

    void createDescriptorLayout();

    void createDescriptorPool();

    void allocateDescriptorSets();

    void createDescriptorSets();
}

#endif //LEARN_VULKAN_DESCRIPTOR_H
