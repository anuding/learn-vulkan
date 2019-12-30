//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#include "Application.h"
#include "Scene.h"
#include "AssetManager.h"
#include <vector>
namespace Engine {
    class Game : public Engine::RenderCore::Application {
    private:
        std::vector<Scene> scenes;
    public:
        const Scene &getCurrentScene() const;

        const std::vector<Scene> &getScenes() const;

        void setScenes(const std::vector<Scene> &scenes);

    public:
        Game();
        Game(Scene &scene);

        void init();
        virtual void update() override;
    };
}

#endif //ENGINE_GAME_H
