add_library(Application Application.cpp)
ADD_LIBRARY(swapchain_helper SwapChain.cpp)
ADD_LIBRARY(shader_helper Shader.cpp)

IF(WIN32)
	MESSAGE(STATUS "current platform: WIN32 ")
	#INCLUDE_DIRECTORIES(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\include\\GLFW)
	#find_library(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\lib-vc2017 glfw3)
	INCLUDE_DIRECTORIES(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\include)
	LINK_DIRECTORIES(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\lib-vc2017)
	

	INCLUDE_DIRECTORIES(D:\\CG_SDKS\\VulkanSDK\\1.1.121.2\\Include)
	LINK_DIRECTORIES(D:\\CG_SDKS\\VulkanSDK\\1.1.121.2\\Lib)

	ADD_EXECUTABLE(engine main.cpp)
	TARGET_COMPILE_DEFINITIONS(engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)


	TARGET_LINK_LIBRARIES(engine Application)

	TARGET_LINK_LIBRARIES(engine glfw3)
	TARGET_LINK_LIBRARIES(engine vulkan-1)
		TARGET_LINK_LIBRARIES(engine swapchain_helper)
	TARGET_LINK_LIBRARIES(engine shader_helper)
	
	ENDIF()