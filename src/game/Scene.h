//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "GameObject.h"
#include <vector>

class Scene {
private:
    std::vector<GameObject> gameObjects;
public:
    const std::vector<GameObject> &getGameObjects() const;
    void addGameObject(GameObject &gameObject);
    void setGameObjects(const std::vector<GameObject> &gameObjects);
};


#endif //ENGINE_SCENE_H
