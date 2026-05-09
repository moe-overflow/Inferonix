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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace inferonix::window;
using namespace inferonix::events;
using namespace inferonix::input;

using namespace std::chrono;

window::window(window_settings& window_settings)
    : _settings(std::move(window_settings)),
      _instance(nullptr),
      _last_frame_time(steady_clock::now())
{
    init();
}

window::~window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (_instance)
        glfwDestroyWindow(_instance);
}


void window::init()
{
    init_glfw();
    init_imgui();
    _initialized = true;
}

void window::init_glfw()
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

    _instance = glfwCreateWindow(_settings.width, _settings.height, _settings.title.c_str(), nullptr, nullptr);
    set_input_pointer_functions(_instance);

    if (_instance == nullptr)
    {
        spdlog::error("Error while creating Window");
        this->destroy();
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

void window::init_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_ViewportsEnable;

    // font
    /*
    {
        io.Fonts->Clear();
        auto const font_file = RESOURCES_PATH "/fonts/IosevkaCharonMono-Light.ttf";

        _settings.font = io.Fonts->AddFontFromFileTTF(
            font_file,
            _settings.font_size,
            nullptr,
            io.Fonts->GetGlyphRangesJapanese()
        );
        io.Fonts->Build();
    }*/

    auto const glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui_ImplGlfw_InitForOpenGL(_instance, true);

}



bool window::should_close() const
{
    return glfwWindowShouldClose(_instance);
}

void window::poll_events()
{
    glfwPollEvents();
}

void window::swap_buffers() const
{
    glfwSwapBuffers(_instance);
}

void window::destroy()
{
    glfwTerminate();
    _instance = nullptr;
}

void window::close() const
{
    glfwSetWindowShouldClose(_instance, GLFW_TRUE);
}

void window::handle_event(event& event)
{
    event_handler::get()->dispatch(event);
}

float window::get_delta_time()
{
    delta_time_point const current_frame_time = steady_clock::now();
    duration<float> const duration = current_frame_time - _last_frame_time;
    _last_frame_time = current_frame_time;
    return duration.count();
}


void window::display() const
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /**/


    for (const auto& layer : _layer_stack)
    {
        layer->on_render();
    }

    /**/

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(_settings.width), static_cast<float>(_settings.height));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


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
        window::handle_event(e);
    }

    void WindowResizeCallback([[maybe_unused]]GLFWwindow* window, int width, int height)
    {
        window_event e(WindowEventType::WindowResize, width, height);
        window::handle_event(e);
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
                input::set_key_down(*key);

                break;

            case GLFW_REPEAT:
                type = KeyEventType::KeyRepeatEvent;
                break;

            case GLFW_RELEASE:
                type = KeyEventType::KeyReleasedEvent;
                input::set_key_up(*key);
                break;

            default:
                spdlog::warn("Key code {} not known", glfw_key);
                return;
        }

        key_event e(*key, type);
        window::handle_event(e);
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
            if (action == GLFW_PRESS) input::set_mouse_key_down(*key);
            else if (action == GLFW_RELEASE) input::set_mouse_key_up(*key);
        }

        auto e = MouseButton{};

        if (action == GLFW_PRESS)
            e = MouseButton(MouseEventType::MouseButtonClicked, button);

        else if (action == GLFW_RELEASE)
            e = MouseButton(MouseEventType::MouseButtonReleased, button);


        window::handle_event(e);
    }

    void CursorCallback(
        [[maybe_unused]] GLFWwindow* window,
        double const x,
        double const y)
    {
        input::set_mouse_position(static_cast<float>(x), static_cast<float>(y));
        MouseCursorMoved e(MouseEventType::MousePointerMoved, static_cast<int>(x), static_cast<int>(y));
        window::handle_event(e);
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
        window::handle_event(e);
    }
} // namespace


void window::set_input_pointer_functions(GLFWwindow* glfw_window)
{
    glfwSetFramebufferSizeCallback(glfw_window, FramebufferSizeCallback);

    glfwSetKeyCallback(glfw_window, KeyCallback);

    glfwSetCursorPosCallback(glfw_window, CursorCallback);
    glfwSetCursorEnterCallback(glfw_window, PointerEnterCallback);
    glfwSetMouseButtonCallback(glfw_window, MouseButtonCallback);

    glfwSetWindowSizeCallback(glfw_window, WindowResizeCallback);
    glfwSetWindowCloseCallback(glfw_window, WindowCloseCallback);
}
