ADD_LIBRARY(ENGINE_CORE #SHARED
        Application.cpp
        Semaphore.cpp
        Pipeline.cpp
        SwapChain.cpp
        Shader.cpp
        Command.cpp
        RenderPass.cpp
        FrameBuffer.cpp
        Queue.cpp
        ValidationLayer.cpp
        Device.cpp
        Vertex.cpp
        Scene.cpp
        Game.cpp
        GameObject.cpp
        Mesh.cpp
        BufferManager.cpp
        VKContext.cpp VKContext.h Instance.cpp Instance.h)


ADD_EXECUTABLE(engine main.cpp)
TARGET_LINK_LIBRARIES(engine ENGINE_CORE)

IF (UNIX)
    MESSAGE(STATUS "Current platform: UNIX ")
    FIND_PACKAGE(glfw3 3.3 REQUIRED)
    FIND_PACKAGE(Vulkan REQUIRED)

    TARGET_COMPILE_DEFINITIONS(engine PRIVATE VK_USE_PLATFORM_XCB_KHR)
    TARGET_INCLUDE_DIRECTORIES(engine PRIVATE Vulkan::Vulkan)
    TARGET_LINK_LIBRARIES(engine glfw)
    TARGET_LINK_LIBRARIES(engine Vulkan::Vulkan)

#    TARGET_COMPILE_DEFINITIONS(ENGINE_CORE PRIVATE VK_USE_PLATFORM_XCB_KHR)
#    TARGET_INCLUDE_DIRECTORIES(ENGINE_CORE PRIVATE Vulkan::Vulkan)
#    TARGET_LINK_LIBRARIES(ENGINE_CORE glfw)
#    TARGET_LINK_LIBRARIES(ENGINE_CORE Vulkan::Vulkan)
ENDIF ()


IF (WIN32)
    MESSAGE(STATUS "current platform: WIN32 ")
    INCLUDE_DIRECTORIES(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\include)
    LINK_DIRECTORIES(D:\\CG_SDKS\\glfw-3.3.bin.WIN64\\lib-vc2017)

    INCLUDE_DIRECTORIES(D:\\CG_SDKS\\VulkanSDK\\1.1.121.2\\Include)
    LINK_DIRECTORIES(D:\\CG_SDKS\\VulkanSDK\\1.1.121.2\\Lib)

	INCLUDE_DIRECTORIES(D:\\CG_SDKS\\glm)


    TARGET_COMPILE_DEFINITIONS(engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
    TARGET_LINK_LIBRARIES(engine glfw3)
    TARGET_LINK_LIBRARIES(engine vulkan-1)
ENDIF ()