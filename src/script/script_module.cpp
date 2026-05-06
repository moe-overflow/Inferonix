#include "script_module.hpp"
#include "script_object.hpp"
#include "scripting_engine.hpp"

#include <spdlog/spdlog.h>
#include <new>

#include "../scene/components.hpp"
#include "../input/input.hpp"
#include "../input/key_codes.hpp"

using namespace Inferonix::Script;

script_module::script_module(std::shared_ptr<asIScriptModule> module) : _instance(std::move(module))
{}

script_module::script_module(script_module&& other) noexcept : _instance(std::move(other._instance))
{}

script_module& script_module::operator=(script_module&& other) noexcept
{
    if (this != &other)
    {
        _instance = std::move(other._instance);
    }
    return *this;
}

script_object script_module::CreateObject(std::string_view name) const
{
    if (!_instance)
    {
        spdlog::error("Cannot create object: module is null");
        return script_object{};
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
        return script_object{};
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
        return script_object{};
    }

    // Create the object using the factory
    asIScriptContext* ctx = _instance->GetEngine()->CreateContext();
    if (!ctx)
    {
        spdlog::error("Failed to create script context");
        return script_object{};
    }

    int result = ctx->Prepare(factory);
    if (result < 0)
    {
        spdlog::error("Failed to prepare script context (Code: {})", result);
        ctx->Release();
        return script_object{};
    }

    result = ctx->Execute();
    if (result < 0)
    {
        spdlog::error("Failed to execute factory function (Code: {})", result);
        ctx->Release();
        return script_object{};
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
        return script_object{};
    }

    return script_object{std::unique_ptr<asIScriptObject, detail::ScriptObjectDeleter>(obj)};
}

bool script_module::IsValid() const
{
    return _instance != nullptr;
}

namespace Inferonix::Script::detail
{
    // storage for entity to transform mapping
    std::unordered_map<uint32_t, Renderer::transform*> g_entity_transforms;


    void ConstructVec3Default(void* memory)
    {
        new(memory) glm::vec3(0.0f);
    }

    void ConstructVec3Copy(void* memory, const glm::vec3& other)
    {
        new(memory) glm::vec3(other);
    }

    void ConstructVec3Splat(void* memory, float v)
    {
        new(memory) glm::vec3(v);
    }

    void ConstructVec3(void* memory, float x, float y, float z)
    {
        new(memory) glm::vec3(x, y, z);
    }

    void DestructVec3(void* memory)
    {}

    glm::vec3& Vec3Assign(glm::vec3& a, const glm::vec3& b)
    {
        a = b;
        return a;
    }

    void Vec3AddGeneric(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        auto const* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a + *b);
    }

    void Vec3SubGeneric(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        auto const* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a - *b);
    }

    void Vec3MulScalarGeneric(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        float const f = gen->GetArgFloat(0);
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a * f);
    }

    void Vec3MulScalarGeneric_r(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        const float f = gen->GetArgFloat(0);
        new(gen->GetAddressOfReturnLocation()) glm::vec3(f * *a);
    }

    void Vec3AddAssignGeneric(asIScriptGeneric* gen)
    {
        auto* a = static_cast<glm::vec3*>(gen->GetObject());
        auto* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        *a += *b;
        gen->SetReturnAddress(a);
    }


    void RegisterGlobals(asIScriptEngine* engine)
    {
        int r;

        r = engine->RegisterObjectType(
            "vec3",
            sizeof(glm::vec3),
            asOBJ_VALUE | asOBJ_APP_CLASS_CDAK
        ); assert(r >= 0);

        r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y)); assert(r >= 0);
        r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z)); assert(r >= 0);

        // Behaviors
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()",
            asFUNCTION(ConstructVec3Default), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)",
            asFUNCTION(ConstructVec3Splat), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)",
            asFUNCTION(ConstructVec3), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)",
            asFUNCTION(ConstructVec3Copy), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_DESTRUCT, "void f()",
            asFUNCTION(DestructVec3), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 &opAssign(const vec3 &in)",
            asFUNCTION(Vec3Assign), asCALL_CDECL_OBJFIRST); assert(r >= 0);


        // --- math operators
        r = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const",
            asFUNCTION(Vec3AddGeneric), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const",
            asFUNCTION(Vec3SubGeneric), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const",
            asFUNCTION(Vec3MulScalarGeneric), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const",
            asFUNCTION(Vec3MulScalarGeneric_r), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3 &in)",
            asFUNCTION(Vec3AddAssignGeneric), asCALL_GENERIC); assert(r >= 0);

        // 2. Register Camera
        r = engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);

        r = engine->RegisterObjectMethod(
            "Camera", "void SetPosition(const vec3 &in)",
            asFUNCTION(+[](Inferonix::Scene::camera* c, const glm::vec3& pos) { c->SetPosition(pos); }),
            asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        r = engine->RegisterObjectMethod(
            "Camera", "void SetOrientation(const vec3 &in)",
            asFUNCTION(+[](Inferonix::Scene::camera* c, const glm::vec3& ori) { c->SetOrientation(ori); }),
            asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        engine->RegisterGlobalFunction(
            "Camera@ GetMainCamera()",
            asFUNCTION(+[]() -> Inferonix::Scene::camera* { return nullptr; }), asCALL_CDECL
        );

        /**/

        r = engine->RegisterEnum("Key"); assert(r >= 0);

        /**/

# define REG_KEY(Name) engine->RegisterEnumValue("Key", #Name, (int)Inferonix::InputSystem::Key::Name)
        REG_KEY(W); REG_KEY(A); REG_KEY(S); REG_KEY(D);
        REG_KEY(UP); REG_KEY(DOWN); REG_KEY(LEFT); REG_KEY(RIGHT);
        REG_KEY(SPACE); REG_KEY(ESC);
#undef REG_KEY


        /**/

        r = engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
        r = engine->RegisterObjectMethod("Transform", "void Translate(const vec3 &in)",
            asFUNCTION(+[](Inferonix::Renderer::transform* t, const glm::vec3& v) {
                if (t) t->Translate(v);
            }), asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        /**/

        engine->RegisterGlobalFunction("bool IsKeyDown(Key key)",
            asFUNCTION(+[](int key) -> bool {
                return Inferonix::InputSystem::input::IsKeyDown(static_cast<Inferonix::InputSystem::Key>(key));
            }), asCALL_CDECL);

        engine->RegisterGlobalFunction("Transform@ GetTransform(uint entity)",
            asFUNCTION(+[](uint32_t entity) -> Inferonix::Renderer::transform* {
                auto it = g_entity_transforms.find(entity);
                return (it != g_entity_transforms.end()) ? it->second : nullptr;
            }), asCALL_CDECL);

        engine->RegisterGlobalFunction("void print(const string& in)",
            asFUNCTION(+[](const std::string& msg) {
                spdlog::info("[Script] {}", msg);
            }), asCALL_CDECL);

        spdlog::info("Registered global AngelScript functions");
    }
}
