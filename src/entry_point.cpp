#include <inferonix_engine.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        auto const config = inferonix::parse_command_line(argc, argv);
        auto engine = inferonix::inferonix_engine{ config };
        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}



