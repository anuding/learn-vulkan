cmake_minimum_required(VERSION 3.12)
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

PROJECT(LVEngine)
ADD_SUBDIRECTORY(common bin)

ADD_EXECUTABLE(LVEngine main.cpp)

target_link_libraries(LVEngine LVECommon)