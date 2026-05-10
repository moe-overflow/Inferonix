#pragma once

#include "../event/event.hpp"
#include "../event/event_handler.hpp"
#include "../ui/layer.hpp"

#include <chrono>

class GLFWwindow;

namespace inferonix::window
{
    using delta_time_point = std::chrono::time_point<std::chrono::steady_clock>;

    struct window_settings
    {
        int width, height;
        std::string title;
        bool full_screen;
        bool v_sync;
    };

    class window
    {

    public:
        explicit window(window_settings& window_settings);

        window(window const&) = delete;
        window(window&&) = delete;

        window& operator=(window const&) = delete;
        window& operator=(window&&) = delete;

        ~window();

        void init();
        void destroy();

        [[nodiscard]] bool should_close() const;
        void swap_buffers() const;
        static void poll_events();
        //[[nodiscard]] bool key_pressed(int key);
        void close() const;

        static void handle_event(events::event& event);

        [[nodiscard]] float get_delta_time();

        void display() const;

        // void process_input();

        template<typename T, typename... Args> T& add_layer(Args&&... args)
        {
            static_assert(std::is_base_of_v<ui::layer, T>);
            auto layer = std::make_shared<T>(std::forward<Args>(args)...);
            T& ref = *layer;
            _layer_stack.emplace_back(layer);
            layer->on_attach();
            return ref;
        }

        template<typename T> auto get_layer() -> T&
        {
            static_assert(std::is_base_of_v<ui::layer, T>);
            for (auto& layer : _layer_stack)
                if (std::dynamic_pointer_cast<T>(layer))
                    return *std::dynamic_pointer_cast<T>(layer);
            throw std::runtime_error("layer not found");
        }

    private:
        bool _initialized = false;
        GLFWwindow* _instance;
        window_settings _settings;

        delta_time_point _last_frame_time{};

        std::vector<std::shared_ptr<ui::layer>> _layer_stack;

    private:
        void init_glfw();
        void init_imgui() const;

        static void set_input_pointer_functions(GLFWwindow* glfw_window);

    };

} // namespace Inferonix::Window
