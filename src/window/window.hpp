#pragma once

#include "../event/event.hpp"
#include "../event/event_handler.hpp"

#include <chrono>

class GLFWwindow;

namespace inferonix::window
{
    using delta_time_point = std::chrono::time_point<std::chrono::steady_clock>;

    struct window_settings
    {
        int width, height;
        std::string title;
        bool full_screen;
        bool v_sync;
    };

    class window
    {

    public:
        explicit window(window_settings& window_settings);

        window(window const&) = delete;
        window(window&&) = delete;

        window& operator=(window const&) = delete;
        window& operator=(window&&) = delete;

        ~window() = default;


        void init();
        void create();
        void destroy();

        [[nodiscard]] bool should_close() const;
        void swap_buffers() const;
        static void poll_events();
        //[[nodiscard]] bool key_pressed(int key);
        void close() const;

        static void handle_event(events::event& event);

        [[nodiscard]] float get_delta_time();

        // void process_input();


    private:
        bool _initialized = false;
        GLFWwindow* _instance;
        window_settings _settings;

        delta_time_point _last_frame_time{};


    private:
        static void set_input_pointer_functions(GLFWwindow* glfw_window);
    };

} // namespace Inferonix::Window
