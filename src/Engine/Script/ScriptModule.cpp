#include "ScriptModule.hpp"
#include "ScriptObject.hpp"
#include "ScriptingEngine.hpp"

#include <spdlog/spdlog.h>

#include "../Scene/Components.hpp"
#include "../Input/Input.hpp"
#include "../Input/KeyCodes.hpp"

using namespace Inferonix::Script;

ScriptModule::ScriptModule(std::shared_ptr<asIScriptModule> module) : _instance(std::move(module))
{}

ScriptModule::ScriptModule(ScriptModule&& other) noexcept : _instance(std::move(other._instance))
{}

ScriptModule& ScriptModule::operator=(ScriptModule&& other) noexcept
{
    if (this != &other)
    {
        _instance = std::move(other._instance);
    }
    return *this;
}

ScriptObject ScriptModule::CreateObject(std::string_view name) const
{
    if (!_instance)
    {
        spdlog::error("Cannot create object: module is null");
        return ScriptObject{};
    }

    // find a factory function for the class
    auto factory_decl = std::string(name) + "@ f()";
    auto* factory = _instance->GetFunctionByDecl(factory_decl.c_str());
    if (!factory)
    {
        spdlog::error("Failed to find factory function '{}' for class '{}'", factory_decl, name);
        return ScriptObject{};
    }

    // Create the object using the factory
    asIScriptContext* ctx = _instance->GetEngine()->CreateContext();
    if (!ctx)
    {
        spdlog::error("Failed to create script context");
        return ScriptObject{};
    }

    auto result = ctx->Prepare(factory);
    if (result < 0)
    {
        spdlog::error("Failed to prepare script context");
        ctx->Release();
        return ScriptObject{};
    }

    result = ctx->Execute();
    if (result < 0)
    {
        spdlog::error("Failed to execute factory function");
        ctx->Release();
        return ScriptObject{};
    }

}

    auto* obj = static_cast<asIScriptObject*>(ctx->GetReturnAddress());
    if (obj)
    {
        obj->AddRef(); // Take ownership
    }
    
    ctx->Release();

    if (!obj)
    {
        spdlog::error("Factory function returned null");
        return ScriptObject{};
    }

    return ScriptObject{std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter>(obj)};
}

bool ScriptModule::IsValid() const
{
    return _instance != nullptr;
}

namespace Inferonix::Script::detail
{
    // storage for entity to transform mapping
    std::unordered_map<uint32_t, Renderer::Transform*> g_entity_transforms;

    void RegisterGlobals(asIScriptEngine* engine)
    {
        // Register the transform type
        int r = engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT);
        assert(r >= 0);

        // Register a helper method to move the transform (wrapper for glm::vec3)
        r = engine->RegisterObjectMethod("Transform", "void Translate(float x, float y, float z)",
            asFUNCTION(+[](Renderer::Transform* t, float x, float y, float z) {
                if (t) t->Translate({ x, y, z });
            }),
            asCALL_CDECL_OBJFIRST
        );
        assert(r >= 0);

        /**/

        engine->RegisterGlobalFunction(
            "bool IsKeyDown(int key)",
            asFUNCTION(+[](int key) -> bool {
                return Inferonix::InputSystem::Input::IsKeyDown(static_cast<Inferonix::InputSystem::Key>(key));
            }),
            asCALL_CDECL
        );

        engine->RegisterGlobalFunction(
            "Transform@ GetTransform(uint entity)",
            asFUNCTION(+[](uint32_t const entity) -> Inferonix::Renderer::Transform* {
                auto const it = g_entity_transforms.find(entity);
                if (it != g_entity_transforms.end())
                {
                    return it->second;
                }
                return nullptr;
            }
                ),
            asCALL_CDECL
        );

        // Register RegisterEntityTransform function
        engine->RegisterGlobalFunction(
            "void RegisterEntityTransform(uint entity, Transform@ transform)",
            asFUNCTION(+[](uint32_t const entity, Inferonix::Renderer::Transform* transform) {
                g_entity_transforms[entity] = transform;
            }),
            asCALL_CDECL
        );

        // Register Print function
        engine->RegisterGlobalFunction(
            "void print(const string& in)",
            asFUNCTION(+[](const std::string& msg) {
                spdlog::info("[Script] {}", msg);
            }),
            asCALL_CDECL
        );

        spdlog::info("Registered global AngelScript functions");
    }
}

