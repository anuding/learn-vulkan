#include <vector>
#include <set>
#include "Application.h"
#include "Shader.h"
#include "Semaphore.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "Command.h"
#include "ValidationLayer.h"
#include "SwapChain.h"
#include "Queue.h"
#include "Device.h"
#include "Instance.h"
#include "Descriptor.h"
#include <thread>

#define GLM_FORCE_RADIANS
namespace Engine::RenderCore {

    Application::Application() {
        initWindow();
        initVulkan();
    }

    void Application::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "EngineTest", nullptr, nullptr);
    }

    void Application::run() {
        mainLoop();
        cleanUp();
    }

    void Application::createSurface() {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void Application::cleanUp() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(device, commandPool, nullptr);
        for (auto framebuffer: swapChainFrameBuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        for (auto imageView: swapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBufferMemories[i], nullptr);
        }

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        //this is a red line
        vkDestroyDevice(device, nullptr);
        if (enableValidationLayers)
            DebugUtils::DestroyDebugUtilsMessengerEXT();

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::initVulkan() {

        InstanceHelper::init();
        DebugUtils::setupDebugMessenger(enableValidationLayers);

        createSurface();

        DeviceHelper::init();

        SwapChainHelper::init();

        RenderPassHelper::init();

        FrameBufferHelper::init();

        DescriptorHelper::init();

        CommandHelper::init();

        SemaphoreHelper::init();

        PipelineHelper::init();
    }

    void Application::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            a = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> work_time = a - b;
            if (work_time.count() < 17) {
                std::chrono::duration<double, std::milli> delta_ms(17 - work_time.count());
                auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
                std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
            }
            b = std::chrono::system_clock::now();

            update();
            drawFrame();
        }
        vkDeviceWaitIdle(device);
    }


    void Application::drawFrame() {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
        uint32_t imageIndex;
        vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(),
                              imageAvailableSemaphores[currentFrame],
                              VK_NULL_HANDLE, &imageIndex);
        updateUniformBuffer(imageIndex);
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command");
        }

        VkPresentInfoKHR presentInfoKhr = {};
        presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchainKhrs[] = {swapChain};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = swapchainKhrs;
        presentInfoKhr.pImageIndices = &imageIndex;
        presentInfoKhr.pResults = nullptr;
        vkQueuePresentKHR(graphicsQueue, &presentInfoKhr);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Application::update() {

    }

    void Application::updateUniformBuffer(uint32_t index) {
        static float degree = 0.0f;
        if (degree > 360)
            degree = 0.0f;
        ShaderHelper::UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f),
                                glm::radians(degree),
                                glm::vec3(0.0f, 0.0f, 1.0f));
        degree += 0.5f;
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(90.0f),
                                    swapChainExtent.width / (float) swapChainExtent.height, 0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;

        void *data;
        vkMapMemory(device, uniformBufferMemories[index], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, uniformBufferMemories[index]);

    }
}