#include "Application.h"
#include <stdexcept>
#include <iostream>
#include <err.h>

int main(){
    Application app;
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