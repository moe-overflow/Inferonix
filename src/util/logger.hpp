#pragma once 

#include <memory>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include "../window/window.hpp"
#include "../ui/console_sink.hpp"

namespace inferonix
{
    inline auto configure_console_sink(const std::function<void(const std::string& message)>& log_callback) -> void
    {
        auto const custom_sink = std::make_shared<ui::console_sink>(log_callback);
        custom_sink->set_pattern("[%Y-%m-%d %T] [%l] %v");
        spdlog::default_logger()->sinks().push_back(custom_sink);
    }

    enum class LOG_TYPE
    {
        INFO,
        WARNING,
        ERROR,
    };

    template<typename ... Args>
    inline auto LOG(LOG_TYPE type, fmt::format_string<Args...> message, Args&&... args) -> void
    {
        switch (type)
        {
            case LOG_TYPE::INFO:
                spdlog::info(message, std::forward<Args>(args)...);
                return;
            case LOG_TYPE::WARNING:
                spdlog::warn(message, std::forward<Args>(args)...);
                return;
            case LOG_TYPE::ERROR:
                spdlog::error(message, std::forward<Args>(args)...);
        }
    }

}

