include_directories(3rd)

IF (UNIX)
    MESSAGE(STATUS "Current platform: UNIX ")

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
            Instance.cpp
            Descriptor.cpp
            AssetManager.cpp
            Asset.cpp
            Surface.cpp
            VKContext.cpp Test.cpp Test.h)
    ADD_EXECUTABLE(engine main.cpp)
    TARGET_LINK_LIBRARIES(engine ENGINE_CORE)

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

    SET(CG_SDK "C:\\Users\\ANUDING\\FD_Software\\CG_SDKS")

    INCLUDE_DIRECTORIES(${CG_SDK}\\glfw-3.3.bin.WIN64\\include)
    LINK_DIRECTORIES(${CG_SDK}\\glfw-3.3.bin.WIN64\\lib-vc2019)

    INCLUDE_DIRECTORIES(${CG_SDK}\\VulkanSDK\\1.1.121.2\\Include)
    LINK_DIRECTORIES(${CG_SDK}\\VulkanSDK\\1.1.121.2\\Lib)

    INCLUDE_DIRECTORIES(${CG_SDK}\\glm)

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
            VKContext.cpp VKContext.h Instance.cpp Instance.h Descriptor.cpp Descriptor.h AssetManager.cpp AssetManager.h Asset.cpp Asset.h Surface.cpp Surface.h Test.cpp Test.h
            Buffer.cpp Buffer.h)

    ADD_EXECUTABLE(engine main.cpp)
    TARGET_LINK_LIBRARIES(engine ENGINE_CORE)

    TARGET_COMPILE_DEFINITIONS(engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
    TARGET_LINK_LIBRARIES(engine glfw3)
    TARGET_LINK_LIBRARIES(engine vulkan-1)
ENDIF ()