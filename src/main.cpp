#include "Application.h"
#include <stdexcept>
#include <iostream>

int main(){
    Engine::RenderCore::Application app;
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
}