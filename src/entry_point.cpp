#include "inferonix_engine.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        auto engine = inferonix::inferonix_engine{};
        engine.init(RESOURCES_PATH "scenes/level01.json");
        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}



