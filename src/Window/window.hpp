#pragma once

#include "EventSystem/event.hpp"
#include "EventSystem/event_handler.hpp"

#include <chrono>

class GLFWwindow;

namespace Inferonix::Window
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

        void init();
        void create();
        void destroy();

        [[nodiscard]] bool should_close() const;
        void swap_buffers();
        static void poll_events();
        [[nodiscard]] bool key_pressed(int key);
        void close();
        
        static void handle_event(Inferonix::EventSystem::event& event);

        [[nodiscard]] float get_delta_time();

        void process_input();

    public:
        window(const window&) = delete;
        window(window&&) = delete;

        window& operator=(const window&) = delete;
        window& operator=(window&&) = delete;

        ~window() = default;

    private:
        bool _initialized = false;
        GLFWwindow* _instance;
        window_settings _settings;

        delta_time_point _last_frame_time {};


    private:
        static void set_input_pointer_functions(GLFWwindow* window);


    };

}