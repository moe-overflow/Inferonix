#pragma once

#include <vector>

#include "../Renderer/RenderEntity.hpp"
#include "../Scene/Camera.hpp"
#include "DemoCamera.hpp"
#include "DemoEntity.hpp"

namespace Inferonix::Project
{
    class Project
    {
    public:
        Project()
        {
            _main_camera = std::make_shared<DemoCamera>();
            _demo_entities.push_back(std::make_shared<DemoEntity>());
        }

        [[nodiscard]] std::shared_ptr<Scene::Camera> GetMainCamera()
        {
            return _main_camera;
        }

        [[nodiscard]] std::vector<std::shared_ptr<DemoEntity>> const& GetEntitiesData() const
        {
            return _demo_entities;
        }

    private:
        std::shared_ptr<DemoCamera> _main_camera;

        std::vector<std::shared_ptr<DemoEntity>> _demo_entities;

        // std::vector<std::shared_ptr<Scene::Camera>> _secondary_cameras;
    };

} // namespace Inferonix::Project
