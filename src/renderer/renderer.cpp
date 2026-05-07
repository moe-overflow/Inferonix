#include "renderer.hpp"

#include "spdlog/spdlog.h"
#include "event/key_event.hpp"
#include "scene/components.hpp"

#include <utility>

using namespace inferonix::renderer;
using namespace inferonix::scene;

renderer::renderer(std::shared_ptr<window::window> window) : _window_instance(std::move(window))
{
#ifndef NDEBUG
    setup_opengl_debug();
#endif

    set_device_specs();
    set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    setup_grid();
}

static void RenderGrid(scene const& scene, RenderEntity const& grid)
{
    // Alpha Blending (for handling transparency)
    {
        // calculate a color by mixing the new pixel with pixel already in buffer
        glEnable(GL_BLEND);

        // use the alpha value of new color to determine opacity
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    grid.shader_program_.use();
    grid.vertex_array_.bind();

    grid.shader_program_.set_uniform("view", scene.get_editor_camera()->get_view());
    grid.shader_program_.set_uniform("projection", scene.get_editor_camera()->get_projection());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    grid.vertex_array_.unbind();
    grid.shader_program_.unuse();

    glDisable(GL_BLEND);

}

void renderer::render(scene::scene& scene)
{
    if (_grid)
        RenderGrid(scene, *_grid);


    //auto const delta_time = _window_instance->GetDeltaTime();
    //_main_camera->Update(delta_time);

    for (auto const view = scene.get_registry().view<mesh_component, transform_component>(); auto entity : view)
    {
        auto const entity_index = static_cast<uint32_t>(entity);
        if (entity_index >= _render_entities.size() || !_render_entities[entity_index])
            create_render_entity(entity, view.get<mesh_component>(entity));

        auto const& render_entity = _render_entities[entity_index];
        render_entity->shader_program_.use();
        render_entity->vertex_array_.bind();

        render_entity->shader_program_.set_uniform("myColor", 0.541f, 0.124f, 0.784f);

        // uniforms
        render_entity->shader_program_.set_uniform(
            "model",
            view.get<transform_component>(entity).get_matrix()
        );
        render_entity->shader_program_.set_uniform("view", scene.get_editor_camera()->get_view());
        render_entity->shader_program_.set_uniform("projection", scene.get_editor_camera()->get_projection());


        glDrawElements(
            GL_TRIANGLES,
            render_entity->index_buffer_.count(),
            GL_UNSIGNED_INT,
            nullptr
        );

        render_entity->vertex_array_.unbind();
        render_entity->shader_program_.unuse();

    }
}

void renderer::create_render_entity(entity const& entity, mesh_component& mesh)
{
    auto render_entity = std::make_unique<RenderEntity>();

    render_entity->vertex_array_.bind();
    render_entity->vertex_buffer_.bind();

    render_entity->vertex_buffer_.buffer_data(mesh.GetVertices());

    render_entity->index_buffer_.bind();
    render_entity->index_buffer_.buffer_data(mesh.GetIndices());

    vertex_buffer_layout layout;
    layout.push(FLOAT, 3); // position
    layout.push(FLOAT, 3); // normal
    render_entity->vertex_array_.add_vertex_buffer(render_entity->vertex_buffer_, layout);


    render_entity->vertex_array_.set_index_buffer(render_entity->index_buffer_);
    render_entity->vertex_buffer_.unbind();
    render_entity->vertex_array_.unbind();

    /**/

    if (auto const entity_index = static_cast<uint32_t>(entity); entity_index >= _render_entities.size())
        _render_entities.resize(entity_index + 1);

    _render_entities[static_cast<uint32_t>(entity)] = std::move(render_entity);

}

void renderer::set_clear_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
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

    spdlog::info("Device Specifications: \n vendor: {}, \n renderer: {}, \n version: {}, \n shading language version: {}",
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
                spdlog::warn("OpenGL warning({}): {}", id, message);
            else if (severity == GL_DEBUG_SEVERITY_HIGH)
                spdlog::error("OpenGL error({}): {}", id, message);
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
    _grid = std::make_unique<RenderEntity>();
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
    auto indices = std::vector<uint32_t> { 0, 1, 2, 2, 3, 0 };


    _grid->vertex_buffer_.bind();
    _grid->vertex_buffer_.buffer_data(vertices);

    _grid->index_buffer_.bind();
    _grid->index_buffer_.buffer_data(indices);

    auto layout = vertex_buffer_layout{};
    layout.push(FLOAT, 3); // position
    layout.push(FLOAT, 3); // normal

    _grid->vertex_array_.bind();
    _grid->vertex_array_.add_vertex_buffer(_grid->vertex_buffer_, layout);
    _grid->vertex_array_.set_index_buffer(_grid->index_buffer_);

    _grid->vertex_array_.unbind();
}



