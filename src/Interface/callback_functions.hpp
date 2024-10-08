#pragma once

#include "window.hpp"
#include "EventSystem/event_handler.hpp"
#include "EventSystem/event.hpp"

namespace Inferonix::Interface {

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void process_input(window &window);

    void window_close_callback(GLFWwindow *window);
    void window_resize_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void cursor_callback(GLFWwindow *window, double x, double y);
    void pointer_enter_callback(GLFWwindow *window, int entered);

}
