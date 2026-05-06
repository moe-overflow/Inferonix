#include "script_object.hpp"
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

script_object::script_object(std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter> instance) 
    : _instance(std::move(instance))
{}

script_object::~script_object()
{}

script_object::script_object(script_object&& other) noexcept : _instance(std::move(other._instance))
{}

script_object& script_object::operator=(script_object&& other) noexcept
{
    if (this != &other)
    {
        _instance = std::move(other._instance);
    }
    return *this;
}

void script_object::CallMethod(std::string_view method_name, std::vector<void*> const& /*arguments*/) const
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

void script_object::CallStart(uint32_t entity) const
{
    if (!_instance) return;

    auto const* type = _instance->GetObjectType();
    static auto decl = "void Start(uint)";
    auto* method = type->GetMethodByDecl(decl);

    if (method)
    {
        auto* ctx = _instance->GetEngine()->CreateContext();
        ctx->Prepare(method);
        ctx->SetObject(_instance.get());
        ctx->SetArgDWord(0, entity);
        ctx->Execute();
        ctx->Release();
    }
}

void script_object::CallUpdate(float delta_time) const
{
    if (!_instance) return;

    auto* type = _instance->GetObjectType();
    static const char* decl = "void Update(float)";
    auto* method = type->GetMethodByDecl(decl);

    if (method)
    {
        auto* ctx = _instance->GetEngine()->CreateContext();
        ctx->Prepare(method);
        ctx->SetObject(_instance.get());
        ctx->SetArgFloat(0, delta_time);
        ctx->Execute();
        ctx->Release();
    }
}


