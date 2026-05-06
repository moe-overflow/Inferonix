#pragma once

#include "scripting_engine.hpp"
#include "../scene/scene.hpp"
#include "../scene/components.hpp"

#include <unordered_map>
#include <filesystem>

namespace Inferonix::Script
{
    class scripting_engine_launcher final
    {
    public:
        scripting_engine_launcher();
        ~scripting_engine_launcher();

        void Launch();
        void Terminate();

        void Start(Scene::Registry& scene_registry);
        void Update(Scene::Registry& scene_registry, float delta_time);

    private:
        scripting_engine _engine;

        // Map from script path to compiled module
        std::unordered_map<std::string, script_module> _module_storage;
    };
}
