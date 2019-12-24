//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#include "Application.h"
#include "Scene.h"
#include <vector>
namespace Engine {
    class Game : public Engine::RenderCore::Application {
    private:
        std::vector<Scene> scenes;
    public:
        Game();
        virtual void update() override;
    };
}

#endif //ENGINE_GAME_H
