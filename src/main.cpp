//#include "Application.h"
#include "Game.h"
#include <stdexcept>
#include <iostream>

int main() {
//    Engine::RenderCore::Application game;
//    Engine::RenderCore::Application application
    Engine::Game game;
    try {
        game.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

}