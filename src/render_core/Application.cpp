#include <vector>
#include <set>
#include <thread>

#include "Application.h"
#include "../utils/ValidationUtil.h"
namespace Engine::RenderCore {

	Application::Application(size_t width, size_t height, const char* windowName, bool enableValidationLayers)
		: WINDOW_WIDTH(width), WINDOW_HEIGHT(height), WINDOW_NAME(windowName), ENABLE_VALIDATION_LAYERS(enableValidationLayers)
	{
		//order matters!
		window.init(this);
		instance.init(this);
		Engine::Utils::ValidationLayerDebugger debugLayer(instance.get());
		surface.init(this);
		physicalDevice.init(this);
		device.init(this);
		swapChain.init(this);
		renderPass.init(this);
		frameBuffer.init(this);

		vertShader.init(this);
		fragShader.init(this);
		vertShader.load("assets/spv_shaders/vert.spv");
		fragShader.load("assets/spv_shaders/frag.spv");

		pipeLine.init(this);
		commandPool.init(this);
		imageAvailableSemaphores.init(this);
		renderFinishedSemaphores.init(this);
		inFlightFences.init(this);
		imagesInFlight.init(this);
		sampler.init(this);

		//tex.init(this,"C:\\Users\\ANUDING\\Pictures\\kfc.jpg");

		instance.printInfo();
		mainLoop();
	}


	void Application::mainLoop() {
		while (!glfwWindowShouldClose(window.get())) {
			glfwPollEvents();
			a = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> work_time = a - b;
			if (work_time.count() < 17) {
				std::chrono::duration<double, std::milli> delta_ms(17 - work_time.count());
				auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			}
			b = std::chrono::system_clock::now();

			//update();
			drawFrame();
		}
		vkDeviceWaitIdle(device.get());
	}


	void Application::drawFrame() {
		vkWaitForFences(device.get(), 1, &inFlightFences.get()[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(device.get(), 1, &inFlightFences.get()[currentFrame]);
		uint32_t imageIndex;
		vkAcquireNextImageKHR(device.get(), swapChain.get(), std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphores.get()[currentFrame],
			VK_NULL_HANDLE, &imageIndex);
		//updateUniformBuffer(imageIndex);
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.pWaitDstStageMask = waitStages;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores.get()[currentFrame] };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores.get()[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandPool.commandBuffers[imageIndex];

		if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, inFlightFences.get()[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command");
		}

		VkPresentInfoKHR presentInfoKhr = {};
		presentInfoKhr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfoKhr.waitSemaphoreCount = 1;
		presentInfoKhr.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchainKhrs[] = { swapChain.get() };
		presentInfoKhr.swapchainCount = 1;
		presentInfoKhr.pSwapchains = swapchainKhrs;
		presentInfoKhr.pImageIndices = &imageIndex;
		presentInfoKhr.pResults = nullptr;
		vkQueuePresentKHR(device.graphicsQueue, &presentInfoKhr);
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
}