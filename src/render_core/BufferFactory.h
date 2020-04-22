//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_BufferHelper_H
#define ENGINE_BufferHelper_H

#include <vector>
#include <vulkan/vulkan.h>
#include "Buffer.h"
#include <memory>

namespace Engine::RenderCore::BufferFactory {

    std::unique_ptr<Buffer> createStagingBuffer(Application* app, VkDeviceSize size);
    std::unique_ptr<Buffer> createMVPUniformBuffer(Application* app);
}


#endif //ENGINE_BufferHelper_H
