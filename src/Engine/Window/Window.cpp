#include "Window.hpp"

#include "spdlog/spdlog.h"
#include <stdexcept>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../EventSystem/KeyEvent.hpp"
#include "../EventSystem/MouseEvent.hpp"
#include "../EventSystem/WindowEvent.hpp"
#include "../Input/KeyCodes.hpp"
#include "../Input/Input.hpp"

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
    SetInputPointerFunctions(_instance);

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
    std::optional<Key> GlfwToKey(int glfw_key_code)
    {
        switch (glfw_key_code)
        {
            case GLFW_KEY_W:
                return Key::W;
            case GLFW_KEY_A:
                return Key::A;
            case GLFW_KEY_S:
                return Key::S;
            case GLFW_KEY_D:
                return Key::D;
            case GLFW_KEY_UP:
                return Key::UP;
            case GLFW_KEY_DOWN:
                return Key::DOWN;
            case GLFW_KEY_LEFT:
                return Key::LEFT;
            case GLFW_KEY_RIGHT:
                return Key::RIGHT;
            case GLFW_KEY_Q:
                return Key::Q;
            case GLFW_KEY_J:
                return Key::J;
            case GLFW_KEY_K:
                return Key::K;
            case GLFW_KEY_L:
                return Key::L;
            case GLFW_KEY_I:
                return Key::I;
            case GLFW_KEY_F1:
                return Key::F1;
            default:
                spdlog::warn("GLFW key code {} not known", glfw_key_code);
                return std::nullopt;
        }
    }

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void WindowCloseCallback(GLFWwindow* window)
    {
        WindowEvent e(WindowEventType::WindowClose);
        Window::HandleEvent(e);
    }

    void WindowResizeCallback(GLFWwindow* window, int width, int height)
    {
        WindowEvent e(WindowEventType::WindowResize, width, height);
        Window::HandleEvent(e);
    }

    void KeyCallback(GLFWwindow* window, int glfw_key, int scan_code, int action, int mods)
    {
        KeyEventType type;
        const auto key = GlfwToKey(glfw_key);

        if (!key)
            return;

        switch (action)
        {
            case GLFW_PRESS:
                type = KeyEventType::KeyPressedEvent;
                Input::SetKeyDown(*key);

                break;

            case GLFW_REPEAT:
                type = KeyEventType::KeyRepeatEvent;
                Input::SetKeyUp(*key);
                break;

            case GLFW_RELEASE:
                type = KeyEventType::KeyReleasedEvent;
                break;

            default:
                spdlog::warn("Key code {} not known", glfw_key);
                return;
        }

        KeyEvent e(*key, type);
        Window::HandleEvent(e);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseButton e;

        if (action == GLFW_PRESS)
            e = MouseButton(MouseEventType::MouseButtonClicked, button);

        else if (action == GLFW_RELEASE)
            e = MouseButton(MouseEventType::MouseButtonReleased, button);


        Window::HandleEvent(e);
    }

    void CursorCallback(GLFWwindow* window, double x, double y)
    {
        MouseCursorMoved e(MouseEventType::MousePointerMoved, static_cast<int>(x), static_cast<int>(y));
        Window::HandleEvent(e);
    }

    void PointerEnterCallback(GLFWwindow* window, int entered)
    {
        Inferonix::EventSystem::MouseEventType type = Inferonix::EventSystem::MouseEventType::None;
        bool in_window = false;

        switch (entered)
        {
            case 1:
            {
                type = Inferonix::EventSystem::MouseEventType::MousePointerEntered;
                in_window = true;
                break;
            }
            case 0:
            {
                type = Inferonix::EventSystem::MouseEventType::MousePointerEntered;
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


void Window::SetInputPointerFunctions(GLFWwindow* glfw_window)
{
    glfwSetFramebufferSizeCallback(glfw_window, FramebufferSizeCallback);

    glfwSetKeyCallback(glfw_window, KeyCallback);

    glfwSetCursorPosCallback(glfw_window, CursorCallback);
    glfwSetCursorEnterCallback(glfw_window, PointerEnterCallback);
    glfwSetMouseButtonCallback(glfw_window, MouseButtonCallback);

    glfwSetWindowSizeCallback(glfw_window, WindowResizeCallback);
    glfwSetWindowCloseCallback(glfw_window, WindowCloseCallback);
}
