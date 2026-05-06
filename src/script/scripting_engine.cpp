#include "scripting_engine.hpp"
#include "script_module.hpp"

#include <spdlog/spdlog.h>
#include <angelscript.h>
#include <functional>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <angelscript/scriptmath/scriptmath.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <angelscript/scriptarray/scriptarray.h>

using namespace Inferonix::Script;

namespace
{
    void AngelscriptCallback(asSMessageInfo const* message, void* /*arguments*/)
    {
        auto severity = std::string{};
        switch (message->type)
        {
            case asMSGTYPE_ERROR:
                severity = "ERROR";
                break;
            case asMSGTYPE_WARNING:
                severity = "WARNING";
                break;
            case asMSGTYPE_INFORMATION:
                severity = "INFO";
                break;
        }
        spdlog::error("[AngelScript] {} ({}): {}", severity, message->section, message->message);
    }
}

void ScriptingEngineDestructor::operator()(asIScriptEngine* engine) const noexcept
{
    if (engine)
        engine->ShutDownAndRelease();
}

scripting_engine::scripting_engine() : _engine(asCreateScriptEngine())
{
    if (!_engine)
    {
        throw std::runtime_error("Failed to create scripting engine!");
    }
    
    if (_engine->SetMessageCallback(asFunctionPtr(AngelscriptCallback), nullptr, asCALL_CDECL) < 0)
    {
        spdlog::warn("Failed to set AngelScript message callback");
    }

    this->Initialize();
}

scripting_engine::scripting_engine(scripting_engine&& other) noexcept : _engine(std::move(other._engine))
{
}

scripting_engine& scripting_engine::operator=(scripting_engine&& other) noexcept
{
    if (this != &other)
    {
        _engine = std::move(other._engine);
    }
    return *this;
}

void scripting_engine::Initialize() const
{
    spdlog::info("Initializing scripting engine with std::string, math and array...");
    RegisterStdString(_engine.get());
    RegisterScriptMath(_engine.get());
    RegisterScriptArray(_engine.get(), true);
}

void scripting_engine::Terminate() const
{
    if (_engine)
    {
        _engine->ShutDownAndRelease();
    }
}

script_module scripting_engine::CompileScript(std::string const& name, std::string const& path) const
{
    if (!_engine)
    {
        spdlog::error("Cannot compile script: engine is null");
        return {};
    }

    auto builder = CScriptBuilder{};
    auto result = builder.StartNewModule(_engine.get(), name.c_str());

    if (result < 0)
    {
        spdlog::error("Failed to start module '{}'", name);
        return {};
    }

    result = builder.AddSectionFromFile(path.c_str());
    if (result < 0)
    {
        spdlog::error("Failed to add script file '{}'", path);
        return {};
    }

    result = builder.BuildModule();
    if (result < 0)
    {
        spdlog::error("Failed to build module '{}'", name);
        return {};
    }

    spdlog::info("Successfully compiled script '{}' from '{}'", name, path);

    auto module_deleter = [](asIScriptModule* module) {
        if (module)
        {
            module->Discard();
        }
    };

    auto const module = std::shared_ptr<asIScriptModule>(builder.GetModule(), module_deleter);
    return script_module{ module };
}

