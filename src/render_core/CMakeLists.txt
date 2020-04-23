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
	 "Sampler.cpp" 
	
	Buffer.cpp
	 "Image.cpp"
	 BufferFactory.cpp
	 ImageFactory.cpp
	 "DescriptorPool.cpp")

target_link_libraries(LVERenderCore LVECommon)
target_link_libraries(LVERenderCore LVEUtil)
