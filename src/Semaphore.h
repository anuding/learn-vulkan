//
// Created by anuding on 2019/12/21.
//
#include <vector>
#include <vulkan/vulkan.h>

namespace Engine::RenderCore {
    namespace SemaphoreHelper {
        void createSyncObjects(VkDevice &device,
                               std::vector<VkSemaphore> &imageAvailableSemaphores,
                               std::vector<VkSemaphore> &renderFinishedSemaphores,
                               std::vector<VkFence> &inFlightFences,
                               size_t MAX_FRAMES_IN_FLIGHT);
    }
}