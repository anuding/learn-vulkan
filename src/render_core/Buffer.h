#include "VKContext.h"
#include <vulkan/vulkan.h>

namespace Engine::RenderCore::Buffer {
	class Buffer {
	public:
		VkStructureType sType;
		VkDeviceSize size;
		VkBufferUsageFlags flags;
		VkSharingMode sharingModel;
		Buffer();
	};
}