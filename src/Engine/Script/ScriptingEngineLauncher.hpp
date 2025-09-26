#pragma once

#include "ScriptingEngine.hpp"
#include "../Asset/AssetRegistry.hpp"
#include "../Scene/Components.hpp"

namespace Inferonix::Script
{
    class ScriptingEngineLauncher final
    {
    public:
        void Launch() const
        {
            _engine.Initialize();
        }

        void Terminate();

        void Start(Asset::Registry& registry)
        {
            for (auto const view = registry.view<Scene::ScriptComponent>(); auto const entity : view)
            {
                auto& component = view.get<Scene::ScriptComponent>(entity);
                if (component.initialized)
                    continue;

                if (auto module = _engine.CompileScript({ /* todo: generate UUID */ }, component.script_path);
                    module.IsValid()
                )
                    _module_storage[component.script_path] = std::move(module);
                else
                    continue;

                auto class_name = std::filesystem::path(component.script_path).parent_path().string();
                component.script_object = _module_storage.at(component.script_path).CreateObject(class_name);
            }
        }

        void Update();

    private:
        ScriptingEngine _engine;

        std::unordered_map<std::string, ScriptModule> _module_storage;

    };
}
