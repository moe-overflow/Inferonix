#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

#include <utility>

namespace inferonix::ui
{
    class console_sink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        using log_callback = std::function<void(const std::string&)>;
        explicit console_sink(log_callback callback_) : _callback(std::move(callback_))
        {}

    private:
        log_callback _callback;

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            auto formatted = spdlog::memory_buf_t{};
            spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
            if (_callback) _callback(fmt::to_string(formatted));
        }
        void flush_() override {}
    };
}

