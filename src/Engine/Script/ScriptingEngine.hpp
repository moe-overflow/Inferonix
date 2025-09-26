#pragma once

#include "ScriptModule.hpp"
#include <angelscript.h>

#include <memory>


namespace Inferonix::Script
{

    struct ScriptingEngineDestructor
    {
        void operator()(asIScriptEngine* engine) const noexcept;
    };


    class ScriptingEngine final
    {
    public:
        ScriptingEngine();
        ~ScriptingEngine() = default;

        ScriptingEngine(ScriptingEngine const&) = delete;
        ScriptingEngine& operator=(ScriptingEngine const&) = delete;

        ScriptingEngine(ScriptingEngine&&) noexcept;
        ScriptingEngine& operator=(ScriptingEngine&&) noexcept;

        /**/


        void Initialize() const;
        void Terminate() const;

        [[nodiscard]] ScriptModule CompileScript(std::string const& name, std::string const& path) const;


    private:
        std::unique_ptr<asIScriptEngine, ScriptingEngineDestructor> _engine;
    };
}

