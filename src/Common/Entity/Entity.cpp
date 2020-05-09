#include "Common/Entity/Entity.h"
#include "Client/Primitive.h"

#include <Shinobu/Renderer/Renderer.h>

Entity::Entity()
    : id(id)
    , m_shader(sh::Shader::Create("res/shaders/Face.glsl"))
{
    m_mesh = sh::VertexArray::Create();
    m_mesh->SetIndexBuffer(Cube::CreateIndexBuffer());
    auto vertices = Cube::CreateVertices(0, 0, 0, 0);
    auto buffer = Cube::CreateVertexBuffer();
    buffer->SetData(vertices.data(), vertices.size() * sizeof(vertices.front()));
    m_mesh->AddVertexBuffer(buffer);
}

void Entity::Draw() const
{
    sh::Renderer::Submit(m_shader, m_mesh, transform.GetWorldMatrix());
}
