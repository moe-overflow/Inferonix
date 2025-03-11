#include "Window.hpp"

#include "spdlog/spdlog.h"
#include <stdexcept>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../EventSystem/KeyEvent.hpp"
#include "../EventSystem/MouseEvent.hpp"
#include "../EventSystem/WindowEvent.hpp"

using namespace Inferonix::Window;
using namespace Inferonix::EventSystem;
using namespace std::chrono;
using namespace Inferonix::InputSystem;

Window::Window(WindowSettings& window_settings)
    : _settings(std::move(window_settings)),
      _instance(nullptr),
      _last_frame_time(steady_clock::now())
{
    Create();
    _initialized = true;
}

void Window::Init()
{
    _initialized = glfwInit();
    if (!_initialized)
        throw std::runtime_error("GLFW initialization failed!");

    spdlog::info("GLFW was initialized successfully");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
}

void Window::Create()
{
    Init();
    _instance = glfwCreateWindow(_settings.Width, _settings.Height, _settings.Title.c_str(), nullptr, nullptr);
    set_input_pointer_functions(_instance);

    if (_instance == nullptr)
    {
        spdlog::error("Error while creating Window");
        this->Destroy();
        return;
    }

    spdlog::info("Window was created successfully");

    glfwMakeContextCurrent(_instance);

    // After making context, glad is going to be initialized:
    // maybe assertion?! from return value of gladLL (int)
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Error while initializing GLAD");
    }
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_instance);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(_instance);
}

void Window::Destroy()
{
    glfwTerminate();
    _instance = nullptr;
}

void Window::Close()
{
    glfwSetWindowShouldClose(_instance, GLFW_TRUE);
}

void Window::HandleEvent(Inferonix::EventSystem::Event& event)
{
    EventHandler::Get()->Dispatch(event);
}

float Window::GetDeltaTime()
{
    delta_time_point current_frame_time = steady_clock::now();
    duration<float> duration = current_frame_time - _last_frame_time;
    _last_frame_time = current_frame_time;
    return duration.count();
}

/**/


// GLFW callback functions
namespace
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void window_close_callback(GLFWwindow* window)
    {
        WindowEvent e(WindowEventType::WindowClose);
        Window::HandleEvent(e);
    }

    void window_resize_callback(GLFWwindow* window, int width, int height)
    {
        WindowEvent e(WindowEventType::WindowResize, width, height);
        Window::HandleEvent(e);
    }

    void key_callback(GLFWwindow* window, int glfw_key, int scan_code, int action, int mods)
    {
        KeyEventType type = KeyEventType::None;
        auto key = *Input::glfw_to_key(glfw_key);

        switch (action)
        {
            case GLFW_PRESS:
                type = KeyEventType::KeyPressedEvent;
                Input::set_key_down(key);

                break;

            case GLFW_REPEAT:
                type = KeyEventType::KeyRepeatEvent;
                Input::set_key_up(key);
                break;

            case GLFW_RELEASE:
                type = KeyEventType::KeyReleasedEvent;
                break;

            default:

                break;
        }

        KeyEvent e(key, type);
        Window::HandleEvent(e);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseButton e;

        if (action == GLFW_PRESS)
            e = MouseButton(mouse_event_type::mouse_button_clicked, button);

        else if (action == GLFW_RELEASE)
            e = MouseButton(mouse_event_type::mouse_button_released, button);


        Window::HandleEvent(e);
    }

    void cursor_callback(GLFWwindow* window, double x, double y)
    {
        MouseCursorMoved e(mouse_event_type::mouse_pointer_moved, static_cast<int>(x), static_cast<int>(y));
        Window::HandleEvent(e);
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

        Inferonix::EventSystem::MouseCursorEntered e(type, in_window);
        Window::HandleEvent(e);
    }
} // namespace


void Window::set_input_pointer_functions(GLFWwindow* glfw_window)
{
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

    glfwSetKeyCallback(glfw_window, key_callback);

    glfwSetCursorPosCallback(glfw_window, cursor_callback);
    glfwSetCursorEnterCallback(glfw_window, pointer_enter_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);

    glfwSetWindowSizeCallback(glfw_window, window_resize_callback);
    glfwSetWindowCloseCallback(glfw_window, window_close_callback);
}
