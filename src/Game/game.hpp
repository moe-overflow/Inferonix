#pragma once

#include "../src/Interface/application.hpp"
#include <spdlog/spdlog.h>

namespace Game
{

	class game : public Engine::Interface::application
	{
	public:
		game();
		~game() = default;
	};

	
}

Engine::Interface::application* create_app()
{
	return new Game::game();
}
