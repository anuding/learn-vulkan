# # Add definition for math library
add_library(Application Application.cpp)
FIND_PACKAGE(glfw3 3.3 REQUIRED)
FIND_PACKAGE(Vulkan REQUIRED)
#ADD_SUBDIRECTORY(/usr/local/include/GLFW)
#INCLUDE_DIRECTORIES(/usr/local/include/GLFW)
ADD_EXECUTABLE(engine main.cpp SwapChain.cpp SwapChain.h)

TARGET_COMPILE_DEFINITIONS(engine PRIVATE VK_USE_PLATFORM_XCB_KHR)
TARGET_INCLUDE_DIRECTORIES(engine PRIVATE Vulkan::Vulkan)
TARGET_LINK_LIBRARIES(engine Application)
TARGET_LINK_LIBRARIES(engine glfw)
TARGET_LINK_LIBRARIES(engine Vulkan::Vulkan)

#include_directories ("${PROJECT_SOURCE_DIR}/src/incc")
#
#
## add the executable
#add_executable (hihi main.cpp)