add_library(LVERenderCore 
	Window.cpp
	Instance.cpp
	Surface.cpp
	PhysicalDevice.cpp
	Device.cpp
	#SwapChain.cpp
)
target_link_libraries(LVERenderCore LVECommon)
target_link_libraries(LVERenderCore LVEUtil)
