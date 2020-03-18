#include <GLFW/glfw3.h>

namespace Engine::RenderCore {
	class Window {
	public:
		Window(uint32_t width, uint32_t height, const char* windowName);
		~Window();
		GLFWwindow* get();
	private:
		GLFWwindow* window;
		const uint32_t WIDTH;
		const uint32_t HEIGHT;
		const char* WINDOW_NAME;
	};
}