#pragma once

#include <vector>

#include "../Scene/camera.hpp"
#include "../Renderer/render_entity.hpp"
#include "demo_entity.hpp"
#include "demo_camera.hpp"

namespace Inferonix::Project
{
    class project
    {
    public:
        project()
        {
            _main_camera = std::make_shared<demo_camera>();
            _demo_entities.push_back(std::make_shared<demo_entity>());
        }

        [[nodiscard]] std::shared_ptr<Scene::camera> get_main_camera()
        {
            return _main_camera;
        }

        [[nodiscard]] const std::vector<std::shared_ptr<demo_entity>>& get_entities_data() const
        {
            return _demo_entities;
        }

    private:

        std::shared_ptr<demo_camera> _main_camera;

        std::vector<std::shared_ptr<demo_entity>> _demo_entities;

        // std::vector<std::shared_ptr<Scene::camera>> _secondary_cameras;
    };

}

