#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../EventSystem/event.hpp"
#include "../EventSystem/event_handler.hpp"

namespace Inferonix::Interface
{
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
	
    private:
        bool _initialized = false;
        GLFWwindow* _instance = nullptr;
        int _width, _height;

		void set_input_pointer_functions(GLFWwindow* window);


	};

}