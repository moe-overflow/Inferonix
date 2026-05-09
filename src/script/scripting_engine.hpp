#pragma once

#include "script_module.hpp"
#include <angelscript.h>

#include <memory>
#include <string>

namespace inferonix::script
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


        void initialize() const;
        void terminate() const;

        [[nodiscard]] script_module compile_script(std::string const& name, std::string const& path) const;

        asIScriptEngine* get_engine() { return _engine.get(); }
        [[nodiscard]] asIScriptEngine* get_engine() const { return _engine.get(); }

    private:
        std::unique_ptr<asIScriptEngine, ScriptingEngineDestructor> _engine;
    };
}

