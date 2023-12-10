#include <pch.h>
#include "Application.h"

int main() {
    try
    {
        Application app{};
        app.Run();
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}