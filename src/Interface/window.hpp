#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Interface
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

	private:
        bool _initialized = false;
        GLFWwindow* _instance = nullptr;
        int _width, _height;

		void set_input_pointer_functions(GLFWwindow* window);


	};

}