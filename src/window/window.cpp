#include "window.hpp"

#include "spdlog/spdlog.h"
#include <stdexcept>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../event/key_event.hpp"
#include "../event/mouse_event.hpp"
#include "../event/window_event.hpp"
#include "../input/key_codes.hpp"
#include "../input/input.hpp"

using namespace Inferonix::Window;
using namespace Inferonix::EventSystem;
using namespace std::chrono;
using namespace Inferonix::InputSystem;

window::window(WindowSettings& window_settings)
    : _settings(std::move(window_settings)),
      _instance(nullptr),
      _last_frame_time(steady_clock::now())
{
    Create();
    _initialized = true;
}

void window::Init()
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

void window::Create()
{
    Init();
    _instance = glfwCreateWindow(_settings.width, _settings.height, _settings.title.c_str(), nullptr, nullptr);
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

bool window::ShouldClose() const
{
    return glfwWindowShouldClose(_instance);
}

void window::PollEvents()
{
    glfwPollEvents();
}

void window::SwapBuffers() const
{
    glfwSwapBuffers(_instance);
}

void window::Destroy()
{
    glfwTerminate();
    _instance = nullptr;
}

void window::Close() const
{
    glfwSetWindowShouldClose(_instance, GLFW_TRUE);
}

void window::HandleEvent(event& event)
{
    event_handler::Get()->Dispatch(event);
}

float window::GetDeltaTime()
{
    delta_time_point const current_frame_time = steady_clock::now();
    duration<float> const duration = current_frame_time - _last_frame_time;
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
            case GLFW_KEY_E:
                return Key::E;
            case GLFW_KEY_F1:
                return Key::F1;
            default:
                spdlog::warn("GLFW key code {} not known", glfw_key_code);
                return std::nullopt;
        }
    }

    void FramebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int const width, int const height)
    {
        glViewport(0, 0, width, height);
    }

    void WindowCloseCallback([[maybe_unused]] GLFWwindow* window)
    {
        window_event e(WindowEventType::WindowClose);
        window::HandleEvent(e);
    }

    void WindowResizeCallback([[maybe_unused]]GLFWwindow* window, int width, int height)
    {
        window_event e(WindowEventType::WindowResize, width, height);
        window::HandleEvent(e);
    }

    void KeyCallback(
        [[maybe_unused]] GLFWwindow* window,
        int glfw_key,
        [[maybe_unused]] int scan_code,
        int const action,
        [[maybe_unused]] int mods)
    {
        KeyEventType type;
        const auto key = GlfwToKey(glfw_key);

        if (!key)
            return;

        switch (action)
        {
            case GLFW_PRESS:
                type = KeyEventType::KeyPressedEvent;
                input::SetKeyDown(*key);

                break;

            case GLFW_REPEAT:
                type = KeyEventType::KeyRepeatEvent;
                break;

            case GLFW_RELEASE:
                type = KeyEventType::KeyReleasedEvent;
                input::SetKeyUp(*key);
                break;

            default:
                spdlog::warn("Key code {} not known", glfw_key);
                return;
        }

        key_event e(*key, type);
        window::HandleEvent(e);
    }

    std::optional<MouseKey> GlfwToMouseKey(int glfw_button)
    {
        switch (glfw_button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:   return MouseKey::LEFT_BUTTON;
            case GLFW_MOUSE_BUTTON_RIGHT:  return MouseKey::RIGHT_BUTTON;
            case GLFW_MOUSE_BUTTON_MIDDLE: return MouseKey::WHEEL_BUTTON;
            default: return std::nullopt;
        }
    }

    void MouseButtonCallback(
        [[maybe_unused]] GLFWwindow* window,
        int const button,
        int const action,
        [[maybe_unused]]int mods)
    {
        if (auto key = GlfwToMouseKey(button)) {
            if (action == GLFW_PRESS) input::SetMouseKeyDown(*key);
            else if (action == GLFW_RELEASE) input::SetMouseKeyUp(*key);
        }

        MouseButton e;

        if (action == GLFW_PRESS)
            e = MouseButton(MouseEventType::MouseButtonClicked, button);

        else if (action == GLFW_RELEASE)
            e = MouseButton(MouseEventType::MouseButtonReleased, button);


        window::HandleEvent(e);
    }

    void CursorCallback(
        [[maybe_unused]] GLFWwindow* window,
        double const x,
        double const y)
    {
        input::SetMousePosition(static_cast<float>(x), static_cast<float>(y));
        MouseCursorMoved e(MouseEventType::MousePointerMoved, static_cast<int>(x), static_cast<int>(y));
        window::HandleEvent(e);
    }

    void PointerEnterCallback([[maybe_unused]] GLFWwindow* window, int const entered)
    {
        auto type = MouseEventType::None;
        bool in_window = false;

        switch (entered)
        {
            case 1:
            {
                type = MouseEventType::MousePointerEntered;
                in_window = true;
                break;
            }
            case 0:
            {
                type = MouseEventType::MousePointerEntered;
                in_window = false;
                break;
            }
            default:
                break;
        }

        MouseCursorEntered e(type, in_window);
        window::HandleEvent(e);
    }
} // namespace


void window::SetInputPointerFunctions(GLFWwindow* glfw_window)
{
    glfwSetFramebufferSizeCallback(glfw_window, FramebufferSizeCallback);

    glfwSetKeyCallback(glfw_window, KeyCallback);

    glfwSetCursorPosCallback(glfw_window, CursorCallback);
    glfwSetCursorEnterCallback(glfw_window, PointerEnterCallback);
    glfwSetMouseButtonCallback(glfw_window, MouseButtonCallback);

    glfwSetWindowSizeCallback(glfw_window, WindowResizeCallback);
    glfwSetWindowCloseCallback(glfw_window, WindowCloseCallback);
}
