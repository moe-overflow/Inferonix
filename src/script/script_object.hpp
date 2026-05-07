#pragma once

#include <memory>
#include <string_view>
#include <vector>
#include <angelscript.h>

namespace inferonix::script
{
    namespace detail
    {
        struct script_object_deleter
        {
            void operator()(asIScriptObject const* obj) const noexcept;
        };
    }

    class script_object final
    {
    public:
        script_object() = default;
        explicit script_object(std::unique_ptr<asIScriptObject, detail::script_object_deleter> instance);
        ~script_object();

        script_object(const script_object&) = delete;
        script_object& operator=(const script_object&) = delete;

        script_object(script_object&&) noexcept;
        script_object& operator=(script_object&&) noexcept;

        void call_method(std::string_view method_name, std::vector<void*> const& arguments = {}) const;

        [[nodiscard]] bool is_valid() const { return _instance != nullptr; }

        [[nodiscard]] asIScriptObject* get_instance() const { return _instance.get(); }

        void call_start(uint32_t entity) const;
        void call_update(float delta_time) const;

    private:
        std::unique_ptr<asIScriptObject, detail::script_object_deleter> _instance;
    };
}

