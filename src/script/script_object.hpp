#pragma once

#include <memory>
#include <string_view>
#include <vector>
#include <angelscript.h>

namespace Inferonix::Script
{
    namespace detail
    {
        struct ScriptObjectDeleter
        {
            void operator()(asIScriptObject const* obj) const noexcept;
        };
    }

    class script_object final
    {
    public:
        script_object() = default;
        explicit script_object(std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> instance);
        ~script_object();

        script_object(const script_object&) = delete;
        script_object& operator=(const script_object&) = delete;

        script_object(script_object&&) noexcept;
        script_object& operator=(script_object&&) noexcept;

        void CallMethod(std::string_view method_name, std::vector<void*> const& arguments = {}) const;

        [[nodiscard]] bool IsValid() const { return _instance != nullptr; }

        [[nodiscard]] asIScriptObject* GetInstance() const { return _instance.get(); }

        void CallStart(uint32_t entity) const;
        void CallUpdate(float delta_time) const;

    private:
        std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> _instance;
    };
}

