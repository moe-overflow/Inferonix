#include "renderer.hpp"

#include <spdlog/spdlog.h>
#include <utility>

using namespace Inferonix::Renderer;

renderer::renderer(std::shared_ptr<Interface::window> window) :
    _window_instance(std::move(window))
{

    log_info();

    // todo: reserve place for vector?

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // for debugging
    glEnable(GL_DEBUG_OUTPUT);

    set_clear_color(0.1f, 0.5f, 0.7f, 1.0f);

}

void renderer::render()
{
    auto delta_time = _window_instance->get_delta_time();
    _main_camera->update(delta_time);

    for(const auto& entity : _render_entities)
    {
        entity->ShaderProgram->use();
        entity->VertexArray->bind();

        // todo: remove to render entity properties
        if(entity->RenderEntityData->dynamically_colored)
            entity->ShaderProgram->set_dynamic_color("myColor");
        else
            entity->ShaderProgram->set_uniform("myColor", 0.5f, 0.5f, 0.5f);

        entity->RenderEntityData->update(delta_time);

        entity->ShaderProgram->set_uniform("model", entity->RenderEntityData->transform.get_matrix());
        entity->ShaderProgram->set_uniform("view", _main_camera->get_view());
        entity->ShaderProgram->set_uniform("projection", _main_camera->get_projection());

        glDrawElements(
            GL_TRIANGLES,
            static_cast<int>(entity->IndexBuffer->count()),
            GL_UNSIGNED_INT,
            nullptr
        );

        entity->VertexArray->unbind();
        entity->ShaderProgram->unuse();
    }
}

void renderer::add_render_entity(const std::shared_ptr<render_entity_data>& data)
{
    _render_entities.emplace_back(create_render_entity(data));
}

void renderer::set_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::log_info() {

    spdlog::info("GPU vendor: {}", reinterpret_cast<char const*> (glGetString(GL_VENDOR)));
    spdlog::info("GPU renderer: {}", reinterpret_cast<char const*> (glGetString(GL_RENDERER)));
    spdlog::info("GPU version: {}", reinterpret_cast<char const*> (glGetString(GL_VERSION)));
    spdlog::info("Shading language: {}", reinterpret_cast<char const*> (glGetString(GL_SHADING_LANGUAGE_VERSION)));

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("Maximum number of vertex attributes supported: {}", nrAttributes);

}

std::shared_ptr<render_entity> renderer::create_render_entity(std::shared_ptr<render_entity_data> data)
{
    auto entity = std::make_shared<render_entity>();

    entity->RenderEntityData = std::move(data);

    entity->ShaderProgram = std::make_unique<shader_program>();
    entity->VertexArray = std::make_unique<vertex_array>();

    entity->VertexBuffer = std::make_unique<vertex_buffer>
    (
        entity->RenderEntityData->vertices.size() * sizeof(float)
    );

    entity->IndexBuffer = std::make_unique<index_buffer>
    (
        entity->RenderEntityData->indices.size() * sizeof(int),
        entity->RenderEntityData->indices.data()
    );

    entity->VertexArray->bind();
    entity->VertexBuffer->bind();
    entity->VertexBuffer->buffer_data
    (
        entity->RenderEntityData->vertices.size() * sizeof(float),
        entity->RenderEntityData->vertices.data()
    );

    entity->IndexBuffer->bind();
    entity->IndexBuffer->buffer_data
    (
        entity->RenderEntityData->indices.size() * sizeof(int),
        entity->RenderEntityData->indices.data()
    );

    vertex_buffer_layout layout1;
    layout1.push(shader_data_type::FLOAT, 3);
    entity->VertexArray->add_vertex_buffer(*entity->VertexBuffer, layout1);

    entity->VertexBuffer->unbind();

    return entity;


}


