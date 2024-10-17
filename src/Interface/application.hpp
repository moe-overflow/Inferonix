#pragma once

#include "window.hpp"
#include "../Renderer/renderer.hpp"

#include <memory>


namespace Inferonix::Interface
{

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
		
	protected:
        std::shared_ptr<Interface::window> _window;
        std::shared_ptr<Renderer::renderer> _renderer;

    };

}

extern Inferonix::Interface::application* create_app();
