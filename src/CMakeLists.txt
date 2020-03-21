cmake_minimum_required(VERSION 3.12)
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR})

PROJECT(LVEngine)
ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(render_core)
ADD_SUBDIRECTORY(game)

ADD_EXECUTABLE(LVEngine main.cpp)

target_link_libraries(LVEngine LVECommon)
target_link_libraries(LVEngine LVEUtil)
target_link_libraries(LVEngine LVERenderCore)
target_link_libraries(LVEngine LVEGame)