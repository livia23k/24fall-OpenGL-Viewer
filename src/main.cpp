#include "viewer/Application.h"
#include <iostream>

int main()
{
    Application app;
    try {
        app.Initialize();
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        app.Clean();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}