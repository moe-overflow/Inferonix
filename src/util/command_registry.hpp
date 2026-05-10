#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

namespace inferonix::utils
{
    class command_registry final
    {
    public:
        using command_function = std::function<void(const std::vector<std::string>&)>;

        command_registry() = default;
        ~command_registry() = default;

        struct console_command
        {
            std::string name;
            command_function action;
        };

        auto register_command(const std::string& name, const std::string& desc, command_function func) -> void
        {
            _commands[name] = {desc, std::move(func)};
        }

        auto execute(const std::string_view command) -> void
        {
            if (command.empty()) return;
            auto const raw = std::string{ command };
            spdlog::info("] {}", raw);

            auto stream = std::istringstream{ raw };
            std::string command_name;
            stream >> command_name;

            auto args = std::vector<std::string>{};
            auto arg = std::string{};
            while (stream >> arg)
                args.push_back(arg);

            handle_command(command_name, args);
        }

        auto handle_command(std::string_view command_name, const std::vector<std::string>& args) -> void
        {
            auto key = std::string{ command_name };
            if (_commands.contains(key)) _commands[key].action(args);
            else spdlog::warn("Unknown command: '{}'", command_name);

        }

    private:
        std::unordered_map<std::string, console_command> _commands;
    };
}

