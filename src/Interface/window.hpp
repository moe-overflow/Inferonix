#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../EventSystem/event.hpp"
#include "../EventSystem/event_handler.hpp"

#include <chrono>

namespace Inferonix::Interface
{
    using delta_time_point = std::chrono::time_point<std::chrono::steady_clock>;

    class window
	{

	public:
		window(int width, int height);
		~window();
        void init();
		void create();
		void destroy();
        GLFWwindow& get();
        [[nodiscard]] bool should_close() const;
        void swap_buffers();
        void poll_events();
        [[nodiscard]] bool key_pressed(int key);
        void close();
        
        static void handle_event(Inferonix::EventSystem::event& event);

        [[nodiscard]] float get_delta_time();

    public:
        window(const window&) = delete;
        window(window&&) = delete;

        window& operator=(const window&) = delete;
        window& operator=(window&&) = delete;

        ~window() = default;

    private:
        bool _initialized = false;
        GLFWwindow* _instance = nullptr;
        int _width, _height;

        delta_time_point _last_frame_time {};


    private:
        void set_input_pointer_functions(GLFWwindow* window);


    };

}