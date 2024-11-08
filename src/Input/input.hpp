#pragma once

#include "key_codes.hpp"

#include <optional>
#include <unordered_map>

namespace Inferonix::Input
{
	class input
	{
	public:

        input() = delete;
        input(const input&) = delete;
        input& operator=(const input&) = delete;


		static bool is_key_down(key key);
		static void set_key_down(key key);
		static void set_key_up(key key);


        static std::optional<key>  glfw_to_key(int glfw_key_code);

    private:
        static std::unordered_map<key, bool> keys;
	
	};
}



