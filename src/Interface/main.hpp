#pragma once

#include "window.hpp"
#include "application.hpp"

#include <memory>

using namespace Engine::Interface;

int main(int argc, char** argv)
{
    auto engine = std::make_unique<application>();
    engine->run();

}



