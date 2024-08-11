#pragma once

#include "../src/Interface/application.hpp"
#include <spdlog/spdlog.h>

namespace Game
{

	class game : public Inferonix::Interface::application
	{
	public:
		game();
		~game() = default;
	};

	
}

Inferonix::Interface::application* create_app()
{
	return new Game::game();
}
