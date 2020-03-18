#include <vulkan/vulkan.h>

namespace Engine::RenderCore {
	class PhysicalDevice
	{
	public:
		PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

		VkPhysicalDevice get();
	private:
		VkPhysicalDevice physicalDevice;

		VkSurfaceKHR surfaceCopy;

		void pickPhysicalDevice(VkInstance instance);

		bool isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);

		bool checkPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	};

}
