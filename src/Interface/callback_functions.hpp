#pragma once

#include "window.hpp"

namespace Engine::Interface {

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void process_input(window &window);

}
