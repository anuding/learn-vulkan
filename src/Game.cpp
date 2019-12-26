//
// Created by anuding on 2019/12/24.
//

#include "Game.h"
#include "Command.h"
#include "VKContext.h"

void Engine::Game::update() {
    Application::update();
}

Engine::Game::Game() {
    init();
}

void Engine::Game::init() {

}

Engine::Game::Game(Scene &scene) {
    scenes.push_back(scene);
    bufferManager.createVertexBuffer(scene.getGameObjects()[0].getMesh().getVertices());
    RenderCore::CommandHelper::createCommandBuffers(
            static_cast<uint32_t >(scene.getGameObjects()[0].getMesh().getVertices().size()));
}

const std::vector<Scene> &Engine::Game::getScenes() const {
    return scenes;
}

void Engine::Game::setScenes(const std::vector<Scene> &scenes) {
    Game::scenes = scenes;
}

const Scene &Engine::Game::getCurrentScene() const {
    return scenes[0];
}
