#pragma once

#include "window.hpp"
#include "../Renderer/renderer.hpp"

#include <memory>

using namespace Inferonix::Renderer;

namespace Inferonix::Interface
{

    static constexpr int WIDTH = 1920;
	static constexpr int HEIGHT = 1080;

	class application
	{
	public:
		application();
        application(application const&) = delete;
        application(application&&) = delete;
		~application() = default;
		void run();
		
		window* get_window() { return _window.get(); }
		renderer* get_renderer() { return _renderer.get(); }
    
	protected:
        std::unique_ptr<window> _window;
        std::unique_ptr<renderer> _renderer;

    };


}

extern Inferonix::Interface::application* create_app();
