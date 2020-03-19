#pragma once

namespace Engine::RenderCore {
	class Application;
	template<class T>
	class VkComp {
	public:
		VkComp() = default;
		T& get() {
			return compData;
		};
		Application* app = nullptr;
		virtual void init(Application* app) = 0;
	private:
		T compData;
	};
}