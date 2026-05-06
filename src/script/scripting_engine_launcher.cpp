#include "scripting_engine_launcher.hpp"

#include "script_module.hpp"
#include "script_object.hpp"

#include <spdlog/spdlog.h>

using namespace Inferonix::Script;

scripting_engine_launcher::scripting_engine_launcher() = default;

scripting_engine_launcher::~scripting_engine_launcher()
{
    Terminate();
}

void scripting_engine_launcher::Launch()
{
    // _engine.Initialize();
    spdlog::info("Launching Scripting Engine and registering globals...");
    detail::RegisterGlobals(_engine.GetEngine());
}

void scripting_engine_launcher::Terminate()
{
    _engine.Terminate();
    _module_storage.clear();
}

void scripting_engine_launcher::Start(Scene::Registry& scene_registry)
{
    // spdlog::info("Starting script system...");
    // detail::RegisterGlobals(_engine.GetEngine());

    auto view = scene_registry.view<Scene::ScriptComponent>();
    for (auto entity : view)
    {
        auto& component = view.get<Scene::ScriptComponent>(entity);
        
        if (component.initialized)
            continue;

        spdlog::info("Loading script: {}", component.script_path);

        // Extract class name from filename
        std::filesystem::path script_path(component.script_path);
        std::string class_name = script_path.stem().string();

        // Compile or get the module
        script_module* p_module = nullptr;
        auto it = _module_storage.find(component.script_path);
        if (it != _module_storage.end())
        {
            p_module = &it->second;
        }
        else
        {
            auto module = _engine.CompileScript(class_name, component.script_path);
            if (!module.IsValid())
            {
                spdlog::error("Failed to compile script: {}", component.script_path);
                continue;
            }
            auto result = _module_storage.emplace(component.script_path, std::move(module));
            p_module = &result.first->second;
        }

        // Create the script object
        auto script_object = p_module->CreateObject(class_name);
        if (!script_object.IsValid())
        {
            spdlog::error("Failed to create script object for class: {}", class_name);
            continue;
        }

        component.script_object = std::move(script_object);
        component.initialized = true;

        // Register entity's transform with scripting system
        auto* transform_component = scene_registry.try_get<Scene::TransformComponent>(entity);
        if (transform_component)
        {
            detail::g_entity_transforms[static_cast<uint32_t>(entity)] = &transform_component->GetTransform();
        }

        component.script_object.CallStart(static_cast<uint32_t>(entity));

        spdlog::info("Script '{}' initialized for entity {}", class_name, static_cast<uint32_t>(entity));
    }
}

void scripting_engine_launcher::Update(Scene::Registry& scene_registry, float const delta_time)
{
    auto view = scene_registry.view<Scene::ScriptComponent>();
    
    for (auto const entity : view)
    {
        auto& component = view.get<Scene::ScriptComponent>(entity);
        if (!component.initialized || !component.script_object.IsValid())
            continue;

        component.script_object.CallUpdate(delta_time);
    }
}

