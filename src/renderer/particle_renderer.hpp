#pragma once

#include "renderer/shader_program.hpp"
#include "renderer/vertex_array.hpp"
#include "scene/components.hpp"

#include <filesystem>

namespace inferonix::renderer
{
    class particle_renderer final
    {
    public:
        particle_renderer() = default;

        auto setup(
            const std::filesystem::path& vertex_shader_path,
            const std::filesystem::path& fragment_shader_path,
            const std::filesystem::path& compute_shader_path
        ) -> void
        {
            _shader_program = shader_program { vertex_shader_path, fragment_shader_path };
            _compute_program = shader_program { compute_shader_path };
        }

        auto render(
            scene::simulation_component& simulation,
            const glm::mat4& model_matrix,
            const glm::mat4& view_matrix,
            const glm::mat4& projection_matrix
        ) -> void
        {
            _shader_program.use();
            _shader_program.set_uniform("model", model_matrix);
            _shader_program.set_uniform("view", view_matrix);
            _shader_program.set_uniform("projection", projection_matrix);

            simulation.storage_buffer.bind_base(simulation.binding_point);
            _vertex_array.bind();
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(simulation.particles.size()));
            _vertex_array.unbind();
        }

        auto initialize_buffers(const scene::simulation_component& simulation) const -> void
        {
            using particle = scene::simulation_component::particle;
            _vertex_array.bind();
            simulation.storage_buffer.bind();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0, 3, GL_FLOAT, GL_FALSE,
                sizeof(particle), reinterpret_cast<void*>(offsetof(particle, position))
            );

            _vertex_array.unbind();
            simulation.storage_buffer.unbind();
        }

        auto dispatch_compute(scene::simulation_component& simulation, float dt) -> void
        {
            _compute_program.use();
            _compute_program.set_uniform("delta_time", dt);
            _compute_program.set_uniform("total_particles", static_cast<int>(simulation.particles.size()));
            _compute_program.set_uniform("drag", simulation.drag);

            simulation.storage_buffer.bind_base(simulation.binding_point);

            const auto num_work_groups =
                (simulation.particles.size() + simulation.workgroup_size - 1) / simulation.workgroup_size;
            glDispatchCompute(static_cast<GLuint>(num_work_groups), 1, 1);

            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        }

    private:
        shader_program _shader_program;
        shader_program _compute_program;
        vertex_array _vertex_array;
    };
}
