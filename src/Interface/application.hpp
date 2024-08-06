#pragma once

#include "window.hpp"
#include "../Renderer/renderer.hpp"

#include <memory>

using namespace Engine::Renderer;

namespace Engine::Interface
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

    private:
        std::unique_ptr<window> _window;
        std::unique_ptr<renderer> _renderer;

    };

}
