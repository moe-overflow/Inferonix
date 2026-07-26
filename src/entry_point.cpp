#include <inferonix_engine.hpp>

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
        inferonix::LOG(inferonix::LOG_TYPE::ERROR, "Error: ", e.what());
        return -1;
    }
}


