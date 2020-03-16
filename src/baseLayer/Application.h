#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>
#include "BufferHelper.h"
#include "VKContext.h"
#include <chrono>

namespace Engine::RenderCore {
    class Application {

    public:


    public:
        std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

    public:
        Application();

        void initWindow();

        void initVulkan();

        void run();

        void mainLoop();

        void cleanUp();

        void createSurface();

        virtual void drawFrame();

        virtual void update() = 0;
    };
}