#pragma once

#include "ScriptObject.hpp"

#include <memory>
#include <angelscript.h>

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

        ScriptObject CreateObject(std::string_view name);

        bool IsValid();

    private:

        friend class ScriptObject;

        std::shared_ptr<asIScriptModule> _instance;
    };
}

