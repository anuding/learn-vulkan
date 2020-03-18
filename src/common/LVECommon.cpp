#include "LVECommon.h"
#include <stdio.h>

void sayHi() {
	if(glfwVulkanSupported())
		printf("Hi vulkan!\n");
	else
		printf("where is vulkan?\n");
}
