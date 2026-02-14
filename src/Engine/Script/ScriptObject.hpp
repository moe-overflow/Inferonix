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

    class ScriptObject final
    {
    public:
        ScriptObject() = default;
        explicit ScriptObject(std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> instance);
        ~ScriptObject();

        ScriptObject(const ScriptObject&) = delete;
        ScriptObject& operator=(const ScriptObject&) = delete;

        ScriptObject(ScriptObject&&) noexcept;
        ScriptObject& operator=(ScriptObject&&) noexcept;

        void CallMethod(std::string_view method_name, std::vector<void*> const& arguments = {}) const;

        [[nodiscard]] bool IsValid() const { return _instance != nullptr; }

        [[nodiscard]] asIScriptObject* GetInstance() const { return _instance.get(); }

    private:
        std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> _instance;
    };
}

