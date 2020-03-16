//
// Created by anuding on 2019/12/24.
//

#include "Scene.h"

const std::vector<GameObject> &Scene::getGameObjects() const {
    return gameObjects;
}

void Scene::setGameObjects(const std::vector<GameObject> &gameObjects) {
    Scene::gameObjects = gameObjects;
}

void Scene::addGameObject(GameObject &gameObject) {
    gameObjects.push_back(gameObject);
}
