#include "ScriptingEngine.hpp"

#include <angelscript.h>
#include <functional>
#include <spdlog/spdlog.h>
#include <angelscript/scriptmath/scriptmath.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>

#include <angelscript/scriptbuilder/scriptbuilder.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <angelscript/scriptarray/scriptarray.h>

using namespace Inferonix::Script;

namespace
{
    void AngelscriptCallback(asSMessageInfo const* message, void* arguments)
    {
        auto severity = std::string{};
        switch (message->type)
        {
            // todo: log message with severity
        }
    }

}

void ScriptingEngineDestructor::operator()(asIScriptEngine* engine) const noexcept
{
    if (engine)
        engine->ShutDownAndRelease();
}

ScriptingEngine::ScriptingEngine() : _engine(asCreateScriptEngine())
{
    if (!_engine || _engine->SetMessageCallback(asFunctionPtr(AngelscriptCallback), nullptr, asCALL_CDECL) < 0)
        throw std::runtime_error("Failed to construct scripting engine!");

    this->Initialize();

}

void ScriptingEngine::Initialize() const
{
    RegisterStdString(_engine.get());
    RegisterScriptMath(_engine.get());
}

void ScriptingEngine::Terminate() const
{
    _engine->ShutDownAndRelease();
}

ScriptModule ScriptingEngine::CompileScript(std::string const& name, std::string const& path) const
{
    auto builder = CScriptBuilder{};
    auto result = builder.StartNewModule(_engine.get(), std::string(name).c_str());

    if (result < 0)
    {
        spdlog::error("Failed to start module");
        return {};
    }

    result = builder.AddSectionFromFile(path.data());
    if (result < 0)
    {
        spdlog::error("Failed to add script file");
        return {};
    }

    result = builder.BuildModule();
    if (result < 0)
    {
        spdlog::error("Failed to build module");
        return {};
    }

    auto module_deleter = [](asIScriptModule* module) {
        if (module) {
            module->Discard();
        }
    };

    auto const module = std::shared_ptr<asIScriptModule>(builder.GetModule(), module_deleter);
    return ScriptModule{ module };

}

