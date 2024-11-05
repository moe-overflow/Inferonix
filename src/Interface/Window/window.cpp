#include "window.hpp"

#include <stdexcept>
#include "spdlog/spdlog.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace Inferonix::Interface;
using namespace Inferonix::EventSystem;
using namespace std::chrono;

window::window(window_settings&  window_settings) :
    _settings(std::move(window_settings)),
    _instance(nullptr),
    _last_frame_time (steady_clock::now())
{
    create();
    _initialized = true;
}

void window::init()
{
    _initialized = glfwInit();
    if(!_initialized)
        throw std::runtime_error("GLFW initialization failed!");

    spdlog::info("GLFW was initialized successfully");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
}

void window::create()
{
    init();
    _instance = glfwCreateWindow(_settings.width, _settings.height, _settings.title.c_str(), nullptr, nullptr);
    set_input_pointer_functions(_instance);

	if (_instance == nullptr)
	{
		spdlog::error("Error while creating window");
		this->destroy();
		return;
	}

    spdlog::info("Window was created successfully");

	glfwMakeContextCurrent(_instance);
    glfwSetFramebufferSizeCallback(_instance, framebuffer_size_callback);

    // After making context, glad is going to be initialized:
	// maybe assertion?! from return value of gladLL (int)
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc> (glfwGetProcAddress)))
	{
		throw std::runtime_error("Error while initializing GLAD");
    }

}

bool window::should_close() const
{
    return glfwWindowShouldClose(_instance);
}

void window::poll_events()
{
    glfwPollEvents();
}

void window::swap_buffers()
{
    glfwSwapBuffers(_instance);
}

void window::destroy()
{
    glfwTerminate();
    _instance = nullptr;
}

GLFWwindow& window::get()
{
    return *_instance;
}

bool window::key_pressed(int key)
{
    return glfwGetKey(_instance, key);
}

void window::close()
{
    glfwSetWindowShouldClose(_instance, GLFW_TRUE);
}

void window::set_input_pointer_functions(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetCursorEnterCallback(window, pointer_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetWindowSizeCallback(window, window_resize_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);
}

void window::handle_event(event &event)
{
    event_handler::get()->dispatch(event);
}

float window::get_delta_time()
{
    delta_time_point current_frame_time = steady_clock::now();
    duration<float> duration = current_frame_time - _last_frame_time;
    _last_frame_time = current_frame_time;
    return duration.count();
}
