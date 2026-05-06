#pragma once

#include "script_module.hpp"
#include <angelscript.h>

#include <memory>
#include <string>

namespace Inferonix::Script
{

    struct ScriptingEngineDestructor
    {
        void operator()(asIScriptEngine* engine) const noexcept;
    };


    class scripting_engine final
    {
    public:
        scripting_engine();
        ~scripting_engine() = default;

        scripting_engine(scripting_engine const&) = delete;
        scripting_engine& operator=(scripting_engine const&) = delete;

        scripting_engine(scripting_engine&&) noexcept;
        scripting_engine& operator=(scripting_engine&&) noexcept;

        /**/


        void Initialize() const;
        void Terminate() const;

        [[nodiscard]] script_module CompileScript(std::string const& name, std::string const& path) const;

        asIScriptEngine* GetEngine() { return _engine.get(); }
        [[nodiscard]] asIScriptEngine* GetEngine() const { return _engine.get(); }

    private:
        std::unique_ptr<asIScriptEngine, ScriptingEngineDestructor> _engine;
    };
}

