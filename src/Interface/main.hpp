#pragma once

#include "window.hpp"
#include "application.hpp"

#include <memory>

using namespace Inferonix::Interface;


int main(int argc, char** argv)
{
    auto engine = create_app();
    engine->run();

}



