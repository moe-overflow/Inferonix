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
            const scene::simulation_component& simulation_component,
            [[maybe_unused]] const glm::mat4& model_matrix,
            const glm::mat4& view_matrix,
            const glm::mat4& projection_matrix
        ) -> void
        {

            _shader_program.use();
            _shader_program.set_uniform("model", model_matrix);
            _shader_program.set_uniform("view", view_matrix);
            _shader_program.set_uniform("projection", projection_matrix);

            glEnable(GL_PROGRAM_POINT_SIZE); // required for gl_PointSize

            simulation_component.storage_buffer.bind_base(0);
            _vertex_array.bind();
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(simulation_component.particles.size()));
            _vertex_array.unbind();
        }

        auto initialize_buffers(const scene::simulation_component& simulation_component) const -> void
        {
            using particle = scene::simulation_component::particle;
            _vertex_array.bind();
            simulation_component.storage_buffer.bind();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0, 3, GL_FLOAT, GL_FALSE,
                sizeof(particle), reinterpret_cast<void*>(offsetof(particle, position))
            );

            _vertex_array.unbind();
            simulation_component.storage_buffer.unbind();

        }

        auto dispatch_compute(const scene::simulation_component& simulation_component, float dt) -> void
        {
            _compute_program.use();
            _compute_program.set_uniform("delta_time", dt);
            _compute_program.set_uniform("total_particles", static_cast<int>(simulation_component.particles.size()));
            _compute_program.set_uniform("drag", simulation.drag);

            // bind the SSBO to binding point 0 for the compute shader
            simulation_component.storage_buffer.bind_base(0);

            const auto num_work_groups = (simulation_component.particles.size() + 63) / 64; // local_size_x = 64
            glDispatchCompute(num_work_groups, 1, 1);

            // Ensure compute shader is done writing before vertex shader starts reading the buffer
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        }

    private:
        shader_program _shader_program;
        shader_program _compute_program;
        vertex_array _vertex_array;
    };
}

