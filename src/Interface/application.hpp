#pragma once

#include "window.hpp"
#include "../Renderer/renderer.hpp"

#include <memory>


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

        application& operator=(const application&) = delete;
        application& operator=(application&&) = delete;

		~application() = default;

        void run();
		
		window* get_window() { return _window.get(); }
		Renderer::renderer* get_renderer() { return _renderer.get(); }


	protected:
        std::shared_ptr<Interface::window> _window;
        std::shared_ptr<Renderer::renderer> _renderer;


    };


}

extern Inferonix::Interface::application* create_app();
