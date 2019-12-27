#include "Game.h"
#include <stdexcept>
#include <iostream>

int main() {
    std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f},
                                            {1.0f, 0.4f, 0.0f}},
                                    {{0.5f,  -0.5f, 0.0f},
                                            {0.0f, 1.0f, 0.0f}},
                                    {{0.5f,  0.5f,  0.0f},
                                            {0.0f, 0.2f, 1.0f}},
                                    {{-0.5f, 0.5f,  0.0f},
                                            {0.9f, 0.9f, 1.0f}}};
    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
    Mesh mesh;
    mesh.setVertices(vertices);
    mesh.setIndices(indices);

    GameObject gameObject;
    gameObject.setMesh(mesh);
    Scene scene;
    scene.addGameObject(gameObject);

    Engine::Game game(scene);
    try {
        game.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}