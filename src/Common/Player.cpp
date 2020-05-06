#include "Common/Player.h"
#include "Common/GameLayer.h"
#include "Client/Primitive.h"

#include <Shinobu/Renderer/Renderer.h>

Player::Player(unsigned id)
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

void Player::Draw() const
{
    sh::Renderer::Submit(m_shader, m_mesh, transform.GetWorldMatrix());
}

MoveCommand::MoveCommand(Player& player, const glm::vec3& pos)
    : m_player(player)
    , m_pos(pos) 
    , m_oldPos(player.transform.GetPosition())
{}

void MoveCommand::Execute()
{
    // TODO: Make this some kind interface that we access
    m_player.transform.SetPosition(m_pos);
}

void MoveCommand::Undo()
{
    m_player.transform.SetPosition(m_oldPos);
}
