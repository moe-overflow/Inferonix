#include "renderer.hpp"

#include <spdlog/spdlog.h>
#include <Game/game.hpp>
#include <utility>

using namespace Inferonix::Renderer;

renderer::renderer()
{
    log_info();

    // todo: reserve place for vector?

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // for debugging
    glEnable(GL_DEBUG_OUTPUT);

}

void renderer::render()
{
    for(const auto& entity : _render_entities)
    {
        entity->ShaderProgram->use();
        entity->VertexArray->bind();
        entity->ShaderProgram->set_dynamic_color("myColor");

        glm::mat4 model = entity->RenderEntityData->transform.get_matrix();
        entity->ShaderProgram->set_uniform("model", model);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f) 
        );
        entity->ShaderProgram->set_uniform("view", view);


        // Set projection matrix (typically the same for all entities in a scene)
        glm::mat4 projection = glm::perspective(
            glm::radians(30.0f), // FOV
            16.0f / 7.0f,
            5.0f, // Near plane
            22.0f // Far plane
        );
        entity->ShaderProgram->set_uniform("projection", projection);


        entity->RenderEntityData->update(2);


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
        entity->RenderEntityData->vertices.size() * sizeof(float),
        entity->RenderEntityData->vertices.data()
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
    ); // todo: redundant (see ctor)

    vertex_buffer_layout layout1;
    layout1.push(shader_data_type::FLOAT, 3);
    entity->VertexArray->add_vertex_buffer(*entity->VertexBuffer, layout1);

    entity->VertexBuffer->unbind();

    return entity;


}


