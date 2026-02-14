#include "ScriptObject.hpp"
#include <spdlog/spdlog.h>

using namespace Inferonix::Script;

namespace Inferonix::Script::detail
{
    void ScriptObjectDeleter::operator()(asIScriptObject const* obj) const noexcept
    {
        if (obj)
        {
            obj->Release();
        }
    }
}

ScriptObject::ScriptObject(std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> instance) 
    : _instance(std::move(instance))
{}

ScriptObject::~ScriptObject()
{}

ScriptObject::ScriptObject(ScriptObject&& other) noexcept : _instance(std::move(other._instance))
{}

ScriptObject& ScriptObject::operator=(ScriptObject&& other) noexcept
{
    if (this != &other)
    {
        _instance = std::move(other._instance);
    }
    return *this;
}

void ScriptObject::CallMethod(std::string_view method_name, std::vector<void*> const& /*arguments*/) const
{
    if (!_instance)
    {
        spdlog::error("Cannot call method: script object is null");
        return;
    }

    auto* type = _instance->GetObjectType();
    
    // Find the method by name
    asIScriptFunction* method = nullptr;
    for (int32_t i = 0; i < type->GetMethodCount(); ++i)
    {
        auto* m = type->GetMethodByIndex(i);
        if (m && std::string(m->GetName()) == method_name)
        {
            method = m;
            break;
        }
    }
    
    if (!method)
    {
        spdlog::error("Method '{}' not found in script object", method_name);
        return;
    }

    // Create a context for calling the method
    auto* engine = _instance->GetEngine();
    auto* ctx = engine->CreateContext();
    if (!ctx)
    {
        spdlog::error("Failed to create script context for method call");
        return;
    }

    auto result = ctx->Prepare(method);
    if (result < 0)
    {
        spdlog::error("Failed to prepare context for method '{}'", method_name);
        ctx->Release();
        return;
    }

    result = ctx->Execute();
    if (result < 0)
    {
        spdlog::error("Failed to execute method '{}'", method_name);
    }

    ctx->Release();
}

