#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>
#include "BufferManager.h"
#include "VKContext.h"
#include <chrono>

namespace Engine::RenderCore {
    class Application {

    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    public:
        Resource::BufferManager bufferManager;
        std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

    public:
        Application();

        void run();

        void initWindow();

        void initVulkan();

        void mainLoop();

        void cleanUp();

        void createSurface();

        virtual void drawFrame();

        virtual void update();

        void updateUniformBuffer(uint32_t index);
    };
}