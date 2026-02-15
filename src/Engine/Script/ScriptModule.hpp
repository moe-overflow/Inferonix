#pragma once

#include "ScriptObject.hpp"

#include <memory>
#include <angelscript.h>
#include <unordered_map>

namespace Inferonix::Renderer
{
    struct Transform;
}

namespace Inferonix::Script
{

    class ScriptModule final
    {
    public:
        ScriptModule() = default;
        explicit ScriptModule(std::shared_ptr<asIScriptModule> module);
        ~ScriptModule() = default;

        ScriptModule(const ScriptModule&) = delete;
        ScriptModule& operator=(const ScriptModule&) = delete;

        ScriptModule(ScriptModule&&) noexcept;
        ScriptModule& operator=(ScriptModule&&) noexcept;

        [[nodiscard]] ScriptObject CreateObject(std::string_view name) const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] asIScriptModule* GetModule() const { return _instance.get(); }

    private:
        std::shared_ptr<asIScriptModule> _instance;
    };

    namespace detail
    {
        void RegisterGlobals(asIScriptEngine* engine);

        // Storage for entity to transform mapping
        extern std::unordered_map<uint32_t, Renderer::Transform*> g_entity_transforms;
    }

}

