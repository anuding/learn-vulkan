#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Engine::RenderCore {
	class Surface {
	public:
		Surface(VkInstance instance, GLFWwindow* window);
		~Surface();
		VkSurfaceKHR get();
	private:
		VkSurfaceKHR surface;
		VkInstance instanceCopy;
		GLFWwindow* windowCopy;

		void createSurface();
	};
}