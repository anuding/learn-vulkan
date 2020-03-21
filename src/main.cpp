#include <iostream>
#include "render_core/Application.h"

int main() {
	Engine::RenderCore::Application app(800, 600, "test", true);
	return 0;
}
//#include "Game.h"
//#include <stdexcept>
//#include <iostream>
//#include "Mesh.h"
//#include "Component.h"
//#include <memory>
//
//class parent
//{
//};
//
//class child : public parent
//{
//	std::vector<uint16_t > indices;
//
//};
//
//int main() {
//	std::vector<Vertex> vertices = { {{-0.5f, -0.5f, 0.0f},
//											{1.0f, 0.4f, 0.0f},{1.0f,0.0f}},
//									{{0.5f,  -0.5f, 0.0f},
//											{0.0f, 1.0f, 0.0f},{0.0f,0.0f}},
//									{{0.5f,  0.5f,  0.0f},
//											{0.0f, 0.2f, 1.0f},{0.0f,1.0f}},
//									{{-0.5f, 0.5f,  0.0f},
//											{0.9f, 0.9f, 1.0f},{1.0f,1.0f}},
//
//									{{-0.5f, -0.5f, -0.5f},
//											{1.0f, 0.4f, 0.0f},{1.0f,0.0f}},
//									{{0.5f,  -0.5f, -0.5f},
//											{0.0f, 1.0f, 0.0f},{0.0f,0.0f}},
//									{{0.5f,  0.5f,  -0.5f},
//											{0.0f, 0.2f, 1.0f},{0.0f,1.0f}},
//									{{-0.5f, 0.5f,  -0.5f},
//											{0.9f, 0.9f, 1.0f},{1.0f,1.0f}},
//	};
//	const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0,
//											4,5,6,6,7,4 };
//
//	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
//	mesh->vertices = vertices;
//	mesh->indices = indices;
//	std::shared_ptr<Componenet> c;
//	c = mesh;
//
//	GameObject gameObject;
//	gameObject.components.push_back(c);
//	Scene scene;
//	scene.addGameObject(gameObject);
//
//	Engine::Game game(scene);
//	try {
//		game.run();
//	}
//	catch (const std::exception & e) {
//		std::cerr << e.what() << '\n';
//		return EXIT_FAILURE;
//	}
//}