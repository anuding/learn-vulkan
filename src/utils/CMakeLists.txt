add_library(LVEUtil 
	ValidationUtil.cpp
	DeviceQueueUtil.cpp 
	"FileUtil.cpp")
target_link_libraries(LVEUtil LVECommon)
