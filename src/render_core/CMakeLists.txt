add_library(LVERenderCore 
	VkComp.cpp

	Application.cpp
	Window.cpp
	Instance.cpp
	Surface.cpp
	PhysicalDevice.cpp
	Device.cpp
	SwapChain.cpp
	RenderPass.cpp
	FrameBuffer.cpp
	Shader.cpp
	Vertex.cpp

	Pipeline.cpp

	CommandPool.cpp
	Semaphore.cpp
	Fence.cpp
	Texture.cpp "Sampler.cpp")
target_link_libraries(LVERenderCore LVECommon)
target_link_libraries(LVERenderCore LVEUtil)
