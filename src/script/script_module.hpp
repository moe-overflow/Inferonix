#pragma once

#include "script_object.hpp"

#include <memory>
#include <angelscript.h>
#include <unordered_map>

namespace inferonix::renderer
{
    struct transform;
}

namespace inferonix::script
{

    class script_module final
    {
    public:
        script_module() = default;
        explicit script_module(std::shared_ptr<asIScriptModule> module);
        ~script_module() = default;

        script_module(const script_module&) = delete;
        script_module& operator=(const script_module&) = delete;

        script_module(script_module&&) noexcept;
        script_module& operator=(script_module&&) noexcept;

        [[nodiscard]] script_object CreateObject(std::string_view name) const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] asIScriptModule* GetModule() const { return _instance.get(); }

    private:
        std::shared_ptr<asIScriptModule> _instance;
    };

    namespace detail
    {
        void register_globals(asIScriptEngine* engine);

        // Storage for entity to transform mapping
        extern std::unordered_map<uint32_t, renderer::transform*> g_entity_transforms;
    }

}

