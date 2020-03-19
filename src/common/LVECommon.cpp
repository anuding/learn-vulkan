#include "LVECommon.h"
#include <stdio.h>

void sayHi() {
	glfwInit();
	if(glfwVulkanSupported())
		printf("Hi vulkan!\n");
	else
		printf("where is vulkan?\n");
}
