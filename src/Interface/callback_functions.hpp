#pragma once

#include "window.hpp"

namespace Inferonix::Interface {

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void process_input(window &window);

}
