#include "script_module.hpp"

#include "inferonix_pch.hpp"

#include "script_object.hpp"
#include "scripting_engine.hpp"

#include <new>

#include "scene/components.hpp"
#include "input/input.hpp"

using namespace inferonix::script;

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
        LOG(LOG_TYPE::ERROR, "Cannot create object: module is null");
        return script_object{};
    }

    asITypeInfo* type_info = _instance->GetTypeInfoByName(name.data());
    if (!type_info)
    {
        LOG(LOG_TYPE::ERROR, "class '{}' not found in script module.", name);

        // DEBUG
        LOG(LOG_TYPE::INFO, "available types in module:");
        for (uint32_t i = 0; i < _instance->GetObjectTypeCount(); ++i)
        {
            auto const* type = _instance->GetObjectTypeByIndex(i);
            LOG(LOG_TYPE::INFO, " - {}", type->GetName());
        }
        return script_object{};
    }

    // Find the default factory function
    std::string factory_decl = std::string(name) + "@ f()";
    asIScriptFunction* factory = type_info->GetFactoryByDecl(factory_decl.c_str());

    if (!factory)
    {
        LOG(LOG_TYPE::ERROR, "Failed to find default factory '{}' for class '{}'", factory_decl, name);

        // DEBUG
        LOG(LOG_TYPE::INFO, "Available constructors:");
        for (uint32_t i = 0; i < type_info->GetFactoryCount(); ++i)
        {
            auto const* f = type_info->GetFactoryByIndex(i);
            LOG(LOG_TYPE::INFO, " - {}", f->GetDeclaration());
        }
        return script_object{};
    }

    // Create the object using the factory
    asIScriptContext* ctx = _instance->GetEngine()->CreateContext();
    if (!ctx)
    {
        LOG(LOG_TYPE::ERROR, "Failed to create script context");
        return script_object{};
    }

    int result = ctx->Prepare(factory);
    if (result < 0)
    {
        LOG(LOG_TYPE::ERROR, "Failed to prepare script context (Code: {})", result);
        ctx->Release();
        return script_object{};
    }

    result = ctx->Execute();
    if (result < 0)
    {
        LOG(LOG_TYPE::ERROR, "Failed to execute factory function (Code: {})", result);
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
        LOG(LOG_TYPE::ERROR, "Factory function returned null");
        return script_object{};
    }

    return script_object{std::unique_ptr<asIScriptObject, detail::script_object_deleter>(obj)};
}

bool script_module::IsValid() const
{
    return _instance != nullptr;
}

namespace inferonix::script::detail
{
    // storage for entity to transform mapping
    std::unordered_map<uint32_t, renderer::transform*> g_entity_transforms;


    void construct_vec_3default(void* memory)
    {
        new(memory) glm::vec3(0.0f);
    }

    void construct_vec3_copy(void* memory, const glm::vec3& other)
    {
        new(memory) glm::vec3(other);
    }

    void construct_vec3_splat(void* memory, float v)
    {
        new(memory) glm::vec3(v);
    }

    void construct_vec3(void* memory, float x, float y, float z)
    {
        new(memory) glm::vec3(x, y, z);
    }

    void destruct_vec3(void* memory)
    {}

    glm::vec3& vec3_assign(glm::vec3& a, const glm::vec3& b)
    {
        a = b;
        return a;
    }

    void vec3_add_generic(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        auto const* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a + *b);
    }

    void vec3_sub_generic(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        auto const* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a - *b);
    }

    void vec3_mul_scalar_generic(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        float const f = gen->GetArgFloat(0);
        new(gen->GetAddressOfReturnLocation()) glm::vec3(*a * f);
    }

    void vec3_mul_scalar_generic_r(asIScriptGeneric* gen)
    {
        auto const* a = static_cast<glm::vec3*>(gen->GetObject());
        const float f = gen->GetArgFloat(0);
        new(gen->GetAddressOfReturnLocation()) glm::vec3(f * *a);
    }

    void vec3_add_assign_generic(asIScriptGeneric* gen)
    {
        auto* a = static_cast<glm::vec3*>(gen->GetObject());
        auto* b = static_cast<glm::vec3*>(gen->GetArgObject(0));
        *a += *b;
        gen->SetReturnAddress(a);
    }


    void register_globals(asIScriptEngine* engine)
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
            asFUNCTION(construct_vec_3default), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float)",
            asFUNCTION(construct_vec3_splat), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)",
            asFUNCTION(construct_vec3), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)",
            asFUNCTION(construct_vec3_copy), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_DESTRUCT, "void f()",
            asFUNCTION(destruct_vec3), asCALL_CDECL_OBJFIRST); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 &opAssign(const vec3 &in)",
            asFUNCTION(vec3_assign), asCALL_CDECL_OBJFIRST); assert(r >= 0);


        // math operators
        r = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const",
            asFUNCTION(vec3_add_generic), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const",
            asFUNCTION(vec3_sub_generic), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const",
            asFUNCTION(vec3_mul_scalar_generic), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const",
            asFUNCTION(vec3_mul_scalar_generic_r), asCALL_GENERIC); assert(r >= 0);

        r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3 &in)",
            asFUNCTION(vec3_add_assign_generic), asCALL_GENERIC); assert(r >= 0);

        // register camera
        r = engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);

        r = engine->RegisterObjectMethod(
            "Camera", "void SetPosition(const vec3 &in)",
            asFUNCTION(+[](scene::camera* c, const glm::vec3& pos) { c->set_position(pos); }),
            asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        r = engine->RegisterObjectMethod(
            "Camera", "void SetOrientation(const vec3 &in)",
            asFUNCTION(+[](scene::camera* c, const glm::vec3& ori) { c->set_orientation(ori); }),
            asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        engine->RegisterGlobalFunction(
            "Camera@ GetMainCamera()",
            asFUNCTION(+[]() -> scene::camera* { return nullptr; }), asCALL_CDECL
        );

        /**/

        r = engine->RegisterEnum("Key"); assert(r >= 0);

        /**/

# define REG_KEY(Name) engine->RegisterEnumValue("Key", #Name, (int)inferonix::input::Key::Name)
        REG_KEY(W); REG_KEY(A); REG_KEY(S); REG_KEY(D);
        REG_KEY(UP); REG_KEY(DOWN); REG_KEY(LEFT); REG_KEY(RIGHT);
        REG_KEY(SPACE); REG_KEY(ESC);
#undef REG_KEY


        /**/

        r = engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
        r = engine->RegisterObjectMethod("Transform", "void Translate(const vec3 &in)",
            asFUNCTION(+[](inferonix::renderer::transform* t, const glm::vec3& v) {
                if (t) t->translate(v);
            }), asCALL_CDECL_OBJFIRST
        ); assert(r >= 0);

        /**/

        engine->RegisterGlobalFunction("bool IsKeyDown(Key key)",
            asFUNCTION(+[](int key) -> bool {
                return inferonix::input::input::is_key_down(static_cast<input::Key>(key));
            }), asCALL_CDECL);

        engine->RegisterGlobalFunction("Transform@ GetTransform(uint entity)",
            asFUNCTION(+[](uint32_t entity) -> inferonix::renderer::transform* {
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
