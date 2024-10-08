#include "callback_functions.hpp"
#include "EventSystem/window_event.hpp"
#include "EventSystem/key_event.hpp"
#include "EventSystem/mouse_event.hpp"

using namespace Inferonix::EventSystem;

namespace Inferonix::Interface
{

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


    void window_close_callback(GLFWwindow* window)
    {
        window_event e(window_event_type::window_close);
        window::handle_event(e);
    }

    void window_resize_callback(GLFWwindow* window, int width, int height)
    {
        window_event e(window_event_type::window_resize, width, height);
        window::handle_event(e);
    }

    void key_callback(GLFWwindow* window,
                      int key,
                      int scan_code,
                      int action,
                      int mods)
    {
        key_event_type type = key_event_type::none;

        switch(action)
        {
            case GLFW_PRESS:
                type = key_event_type::key_pressed_event;
                break;

            case GLFW_REPEAT:
                type = key_event_type::key_repeat_event;
                break;

            case GLFW_RELEASE:
                type = key_event_type::key_released_event;
                break;

            default:

                break;
        }

        key_event e (key, type);
        window::handle_event(e);

    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        mouse_button e;

        if (action == GLFW_PRESS)
            e = mouse_button(mouse_event_type::mouse_button_clicked, button);

        else if(action == GLFW_RELEASE)
            e = mouse_button(mouse_event_type::mouse_button_released, button);


        window::handle_event(e);

    }

    void cursor_callback(GLFWwindow *window, double x, double y)
    {
        mouse_cursor_moved e(mouse_event_type::mouse_pointer_moved,
                                     static_cast<int>(x),
                                     static_cast<int>(y));
        window::handle_event(e);
    }

    void pointer_enter_callback(GLFWwindow* window, int entered)
    {
        Inferonix::EventSystem::mouse_event_type type = Inferonix::EventSystem::mouse_event_type::none;
        bool in_window = false;

        switch (entered)
        {
            case 1:
            {
                type = Inferonix::EventSystem::mouse_event_type::mouse_pointer_entered;
                in_window = true;
                break;
            }
            case 0:
            {
                type = Inferonix::EventSystem::mouse_event_type::mouse_pointer_entered;
                in_window = false;
                break;
            }
            default:
                break;
        }

        Inferonix::EventSystem::mouse_cursor_entered e(type, in_window);
        window::handle_event(e);

    }

}
