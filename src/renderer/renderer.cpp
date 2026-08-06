#include "renderer.hpp"

#include "inferonix_pch.hpp"
#include "event/key_event.hpp"
#include "scene/components.hpp"
#include "util/time.hpp"
#include <utility>

#include "frame_buffer.hpp"
#include "color.hpp"
#include "scene/model.hpp"

using namespace inferonix::renderer;
using namespace inferonix::scene;

namespace
{
    void render_grid(scene const& scene, render_entity& grid)
    {
        // Alpha Blending (for handling transparency)
        {
            // calculate a color by mixing the new pixel with pixel already in buffer
            glEnablei(GL_BLEND, 0);

            // use the alpha value of new color to determine opacity
            glBlendFuncSeparatei(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        }

        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        grid.shader_program_.use();

        for (auto& grid_gpu_mesh : grid.meshes)
        {
            grid_gpu_mesh.vertex_array_.bind();

            grid.shader_program_.set_uniform("view", scene.get_editor_camera()->get_view());
            grid.shader_program_.set_uniform("projection", scene.get_editor_camera()->get_projection());

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            grid_gpu_mesh.vertex_array_.unbind();
            grid.shader_program_.unuse();
        }

        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
        glDisablei(GL_BLEND, 0);
    }

    auto assign_entity_uniforms(
        shader_program& shader,
        const scene& scene,
        transform_component const& transform,
        material_component const& override_material,
        material const& imported_material,
        entity entity
    )  -> void
    {
        shader.set_uniform("model", transform.get_matrix());
        shader.set_uniform("view", scene.get_editor_camera()->get_view());
        shader.set_uniform("projection", scene.get_editor_camera()->get_projection());

        shader.set_uniform("u_time", inferonix::utils::time::get_time());

        shader.set_uniform("color", imported_material.base_color);

        shader.set_uniform("use_dynamic_color", override_material.use_dynamic_color ? 1 : 0);
        shader.set_uniform("u_entity_id", static_cast<int>(entity));

        if (imported_material.albedo_texture)
        {
            imported_material.albedo_texture->bind(0);
            shader.set_uniform("albedo_map", 0);
            shader.set_uniform("use_texture", true);
        }
        else
        {
            shader.set_uniform("use_texture", false);
            shader.set_uniform("color", imported_material.base_color);
        }
    }


}

renderer::renderer(std::shared_ptr<window::window> window) :
    _window_instance(std::move(window)),
    _frame_buffer(std::make_shared<frame_buffer>(frame_buffer::frame_buffer_settings(1920, 1080)))
{}

void renderer::setup()
{
#ifndef NDEBUG
    setup_opengl_debug();
#endif

    set_device_specs();
    set_clear_color( {.1f, .1f, .1f, 1.0f} );
    glEnable(GL_DEPTH_TEST);
    setup_grid();
}

void renderer::render(scene::scene& scene)
{
    _frame_buffer->bind();
    clear();
    _frame_buffer->clear_attachment(1, -1);

    if (_grid)
        render_grid(scene, *_grid);

    auto const view = scene.get_registry().view<mesh_component, transform_component, material_component>();
    for (auto entity : view)
    {
        auto const entity_index = static_cast<uint32_t>(entity);
        if (entity_index >= _render_entities.size() || !_render_entities[entity_index])
            create_render_entity(entity, view.get<mesh_component>(entity));

        auto const& render_entity = _render_entities[entity_index];
        render_entity->shader_program_.use();

        for (auto& gpu_mesh : render_entity->meshes)
        {
            gpu_mesh.vertex_array_.bind();

            auto const& mesh_component_ref = view.get<mesh_component>(entity);
            auto const& imported_material = mesh_component_ref.model_asset->materials()[gpu_mesh.material_index];

            assign_entity_uniforms(
                render_entity->shader_program_,
                scene,
                view.get<transform_component>(entity),
                view.get<material_component>(entity),
                imported_material,
                entity
            );

            glDrawElements(
                GL_TRIANGLES,
                gpu_mesh.index_buffer_.count(),
                GL_UNSIGNED_INT,
                nullptr
            );

            gpu_mesh.vertex_array_.unbind();
        }
        render_entity->shader_program_.unuse();

    }

    _frame_buffer->unbind();
}

void renderer::create_render_entity(const entity& entity, mesh_component& mesh_component)
{
    auto render_entity_ = std::make_unique<render_entity>();
    render_entity_->meshes.reserve(mesh_component.model_asset->meshes().size());
    for (auto const& [_vertices, _indices, material_index] : mesh_component.model_asset->meshes())
    {
        auto& gpu_m = render_entity_->meshes.emplace_back();

        gpu_m.vertex_array_.bind();
        gpu_m.vertex_buffer_.bind();
        gpu_m.vertex_buffer_.buffer_data(_vertices);

        gpu_m.index_buffer_.bind();
        gpu_m.index_buffer_.buffer_data(_indices);

        gpu_m.material_index = material_index;

        vertex_buffer_layout layout;
        layout.push(FLOAT, 3); // position
        layout.push(FLOAT, 3); // normal
        layout.push(FLOAT, 2); // texture

        gpu_m.vertex_array_.add_vertex_buffer(gpu_m.vertex_buffer_, layout);
        gpu_m.vertex_array_.set_index_buffer(gpu_m.index_buffer_);

        gpu_m.vertex_buffer_.unbind();
        gpu_m.vertex_array_.unbind();
    }

    /**/

    if (auto const entity_index = static_cast<uint32_t>(entity); entity_index >= _render_entities.size())
        _render_entities.resize(entity_index + 1);

    _render_entities[static_cast<uint32_t>(entity)] = std::move(render_entity_);

}

void renderer::set_clear_color(color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::set_device_specs()
{
    _device_specs.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    _device_specs.renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    _device_specs.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    _device_specs.shading_language_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    int max_vertex_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attributes);
    _device_specs.nr_attributes = max_vertex_attributes;

    LOG(LOG_TYPE::INFO, "Device Specifications: \n vendor: {}, \n renderer: {}, \n version: {}, \n shading language version: {}",
        _device_specs.vendor, _device_specs.renderer, _device_specs.version, _device_specs.shading_language_version);
}

void renderer::setup_opengl_debug()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([] (
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
                         GLenum const severity,
        [[maybe_unused]] GLsizei length,
                         const GLchar* message,
        [[maybe_unused]] const void* userParam) -> void
        {
            if (severity == GL_DEBUG_SEVERITY_MEDIUM)
                LOG(LOG_TYPE::WARNING, "OpenGL warning({}): {}", id, message);
            else if (severity == GL_DEBUG_SEVERITY_HIGH)
                LOG(LOG_TYPE::ERROR, "OpenGL error({}): {}", id, message);
        },
        nullptr
    );
}

void renderer::on_event(events::event& event)
{
    if (auto const type = dynamic_cast<events::key_event*>(&event))
    {
        if(type->GetKey() == input::Key::F1 && type->GetType() == events::KeyEventType::KeyPressedEvent)
        {
            _wireframe_mode = !_wireframe_mode;
            glPolygonMode(GL_FRONT_AND_BACK, _wireframe_mode ? GL_LINE : GL_FILL);
        }
    }
}

void renderer::setup_grid()
{
    _grid = std::make_unique<render_entity>();
    _grid->shader_program_ = shader_program{
        SHADERS_PATH "/grid_vertex.glsl",
        SHADERS_PATH "/grid_fragment.glsl"
    };

    auto const vertices = std::vector<vertex>{
        {{-50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}},
        {{-50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}}
    };
    const auto indices = std::vector<uint32_t> { 0, 1, 2, 2, 3, 0 };

    _grid->meshes.emplace_back();
    auto& mesh = _grid->meshes.back();

    mesh.vertex_buffer_.bind();
    mesh.vertex_buffer_.buffer_data(vertices);

    mesh.index_buffer_.bind();
    mesh.index_buffer_.buffer_data(indices);

    auto layout = vertex_buffer_layout{};
    layout.push(FLOAT, 3); // position
    layout.push(FLOAT, 3); // normal
    layout.push(FLOAT, 2); // texture coordinates

    mesh.vertex_array_.bind();
    mesh.vertex_array_.add_vertex_buffer(mesh.vertex_buffer_, layout);
    mesh.vertex_array_.set_index_buffer(mesh.index_buffer_);

}



