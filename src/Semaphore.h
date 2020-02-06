//
// Created by anuding on 2019/12/21.
//

#ifndef ENGINE_SEMAPHORE_H
#define ENGINE_SEMAPHORE_H
#include <vector>
#include <vulkan/vulkan.h>
#include "VKContext.h"

namespace Engine::RenderCore::SemaphoreHelper {

    void init();

    void createSyncObjects();
}
#endif