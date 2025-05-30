#pragma once

#include "../EventSystem/Event.hpp"
#include "../EventSystem/EventHandler.hpp"

#include <chrono>

class GLFWwindow;

namespace Inferonix::Window
{
    using delta_time_point = std::chrono::time_point<std::chrono::steady_clock>;

    struct WindowSettings
    {
        int width, height;
        std::string title;
        bool full_screen;
        bool v_sync;
    };

    class Window
    {

    public:
        explicit Window(WindowSettings& window_settings);

        Window(Window const&) = delete;
        Window(Window&&) = delete;

        Window& operator=(Window const&) = delete;
        Window& operator=(Window&&) = delete;

        ~Window() = default;


        void Init();
        void Create();
        void Destroy();

        [[nodiscard]] bool ShouldClose() const;
        void SwapBuffers();
        static void PollEvents();
        //[[nodiscard]] bool key_pressed(int key);
        void Close();

        static void HandleEvent(Inferonix::EventSystem::Event& event);

        [[nodiscard]] float GetDeltaTime();

        // void process_input();


    private:
        bool _initialized = false;
        GLFWwindow* _instance;
        WindowSettings _settings;

        delta_time_point _last_frame_time{};


    private:
        static void SetInputPointerFunctions(GLFWwindow* glfw_window);
    };

} // namespace Inferonix::Window
