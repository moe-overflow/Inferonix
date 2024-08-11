#include "callback_functions.hpp"

namespace Inferonix::Interface{

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }


    void process_input(window& window)
    {
        if( window.key_pressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }
    }


}