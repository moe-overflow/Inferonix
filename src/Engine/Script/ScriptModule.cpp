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

    asITypeInfo* type_info = _instance->GetTypeInfoByName(name.data());
    if (!type_info)
    {
        spdlog::error("class '{}' not found in script module.", name);

        // DEBUG
        spdlog::info("available types in module:");
        for (uint32_t i = 0; i < _instance->GetObjectTypeCount(); ++i)
        {
            auto const* type = _instance->GetObjectTypeByIndex(i);
            spdlog::info(" - {}", type->GetName());
        }
        return ScriptObject{};
    }

    // Find the default factory function
    std::string factory_decl = std::string(name) + "@ f()";
    asIScriptFunction* factory = type_info->GetFactoryByDecl(factory_decl.c_str());

    if (!factory)
    {
        spdlog::error("Failed to find default factory '{}' for class '{}'", factory_decl, name);

        // DEBUG
        spdlog::info("Available constructors:");
        for (uint32_t i = 0; i < type_info->GetFactoryCount(); ++i)
        {
            auto const* f = type_info->GetFactoryByIndex(i);
            spdlog::info(" - {}", f->GetDeclaration());
        }
        return ScriptObject{};
    }

    // Create the object using the factory
    asIScriptContext* ctx = _instance->GetEngine()->CreateContext();
    if (!ctx)
    {
        spdlog::error("Failed to create script context");
        return ScriptObject{};
    }

    int result = ctx->Prepare(factory);
    if (result < 0)
    {
        spdlog::error("Failed to prepare script context (Code: {})", result);
        ctx->Release();
        return ScriptObject{};
    }

    result = ctx->Execute();
    if (result < 0)
    {
        spdlog::error("Failed to execute factory function (Code: {})", result);
        ctx->Release();
        return ScriptObject{};
    }

    auto* obj = static_cast<asIScriptObject*>(ctx->GetReturnAddress());
    if (obj)
    {
        obj->AddRef();
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

    void ConstructVec3(void* memory, float x, float y, float z)
    {
        new(memory) glm::vec3(x, y, z);
    }

    void ConstructVec3Default(void* memory)
    {
        new(memory) glm::vec3(.0f);
    }

    void RegisterGlobals(asIScriptEngine* engine)
    {
        // Register the transform type
        int r = engine->RegisterObjectType(
            "vec3",
            sizeof(glm::vec3),
            asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA
        ); assert(r >= 0);

        /**/

        r = engine->RegisterObjectProperty(
            "vec3", "float x", asOFFSET(glm::vec3, x)
        ); assert(r >= 0);

        r = engine->RegisterObjectProperty(
            "vec3", "float y", asOFFSET(glm::vec3, y)
        ); assert(r >= 0);

        r = engine->RegisterObjectProperty(
            "vec3", "float z", asOFFSET(glm::vec3, z)
        ); assert(r >= 0);

        /**/

        r = engine->RegisterObjectBehaviour(
            "vec3",asBEHAVE_CONSTRUCT, "void f()",
            asFUNCTION(ConstructVec3Default), asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        r = engine->RegisterObjectBehaviour(
            "vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)",
            asFUNCTION(ConstructVec3), asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        /**/

        r = engine->RegisterEnum("Key"); assert(r >= 0);

        /**/

# define REG_KEY(Name) engine->RegisterEnumValue("Key", #Name, (int)Inferonix::InputSystem::Key::Name)
        REG_KEY(W); REG_KEY(A); REG_KEY(S); REG_KEY(D);
        REG_KEY(UP); REG_KEY(DOWN); REG_KEY(LEFT); REG_KEY(RIGHT);
        REG_KEY(SPACE); REG_KEY(ESC);
#undef REG_KEY


        /**/

        r = engine->RegisterObjectType(
            "Transform", 0, asOBJ_REF | asOBJ_NOCOUNT
        ); assert(r >= 0);

        r = engine->RegisterObjectMethod("Transform", "void Translate(const vec3 &in)",
            asFUNCTION(+[](Inferonix::Renderer::Transform* t, const glm::vec3& v) {
                if (t) t->Translate(v);
            }), asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        /**/

        engine->RegisterGlobalFunction("bool IsKeyDown(Key key)",
            asFUNCTION(+[](int key) -> bool {
                return Inferonix::InputSystem::Input::IsKeyDown(static_cast<Inferonix::InputSystem::Key>(key));
            }), asCALL_CDECL
        );

        engine->RegisterGlobalFunction("Transform@ GetTransform(uint entity)",
            asFUNCTION(+[](uint32_t entity) -> Inferonix::Renderer::Transform* {
                auto it = g_entity_transforms.find(entity);
                return (it != g_entity_transforms.end()) ? it->second : nullptr;
            }), asCALL_CDECL
        );

        engine->RegisterGlobalFunction("void print(const string& in)",
            asFUNCTION(+[](const std::string& msg) {
                spdlog::info("[Script] {}", msg);
            }), asCALL_CDECL
        );

        spdlog::info("Registered global AngelScript functions");
    }
}

