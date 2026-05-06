#pragma once

#include "../event/event.hpp"
#include "../event/event_handler.hpp"

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

    class window
    {

    public:
        explicit window(WindowSettings& window_settings);

        window(window const&) = delete;
        window(window&&) = delete;

        window& operator=(window const&) = delete;
        window& operator=(window&&) = delete;

        ~window() = default;


        void Init();
        void Create();
        void Destroy();

        [[nodiscard]] bool ShouldClose() const;
        void SwapBuffers() const;
        static void PollEvents();
        //[[nodiscard]] bool key_pressed(int key);
        void Close() const;

        static void HandleEvent(EventSystem::event& event);

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
