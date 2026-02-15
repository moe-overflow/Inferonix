#pragma once

#include "ScriptingEngine.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/Components.hpp"

#include <unordered_map>
#include <filesystem>

namespace Inferonix::Script
{
    class ScriptingEngineLauncher final
    {
    public:
        ScriptingEngineLauncher();
        ~ScriptingEngineLauncher();

        void Launch();
        void Terminate();

        void Start(Scene::Registry& scene_registry);
        void Update(Scene::Registry& scene_registry, float delta_time);

    private:
        ScriptingEngine _engine;

        // Map from script path to compiled module
        std::unordered_map<std::string, ScriptModule> _module_storage;
    };
}
