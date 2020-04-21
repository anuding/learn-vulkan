#include <iostream>
#include <vector>
#include <chrono>

#include "Window.h"
#include "Surface.h"
#include "PhysicalDevice.h"
#include "FrameBuffer.h"
#include "SwapChain.h"
#include "Device.h"
#include "Instance.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "Shader.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Fence.h"
#include "Sampler.h"
#include "Texture.h"

namespace Engine::RenderCore {
	class Application {
	public:
		//order matters!
		Window window;

		Instance instance;
		Surface surface;

		PhysicalDevice physicalDevice;
		Device device;

		SwapChain swapChain;
		RenderPass renderPass;
		FrameBuffer frameBuffer;
		Shader vertShader;
		Shader fragShader;
		Pipeline pipeLine;
		CommandPool commandPool;
		Semaphore imageAvailableSemaphores;
		Semaphore renderFinishedSemaphores;
		Fence inFlightFences;
		Fence imagesInFlight;
		Sampler sampler;
		Texture tex;

		size_t currentFrame = 0;
		const int MAX_FRAMES_IN_FLIGHT = 2;

		const size_t WINDOW_WIDTH;
		const size_t WINDOW_HEIGHT;
		const char* WINDOW_NAME;
		const bool ENABLE_VALIDATION_LAYERS;

	private:
		std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

	public:
		Application(
			size_t width, 
			size_t height, 
			const char* windowName, 
			bool enableValidationLayers);
		void mainLoop();
		void drawFrame();
	};
}