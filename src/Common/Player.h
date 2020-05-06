#pragma once

#include "Common/Command.h"

#include <Shinobu/Core/Transform.h>
#include <Shinobu/Renderer/VertexArray.h>
#include <Shinobu/Renderer/VertexBuffer.h>
#include <Shinobu/Renderer/Shader.h>

#include "Net/Packet.h"

class Player
{
public:
    Player(unsigned id);

    void Draw() const;

public:
    const unsigned id;
    
    sh::Transform transform;

private:
    sh::ShaderRef m_shader;
    sh::VertexArrayRef m_mesh;
};

class MoveCommand : public Command
{
public:
    MoveCommand(Player& player, const glm::vec3& pos);

    virtual void Execute() override final;
    virtual void Undo() override final;

//private:
    Player& m_player;
    const glm::vec3 m_pos;
    const glm::vec3 m_oldPos;
};

inline sf::Packet& operator<<(sf::Packet& packet, const glm::vec3& vec)
{
    packet << vec.x << vec.y << vec.z;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, glm::vec3& vec)
{
    packet >> vec.x >> vec.y >> vec.z;
    return packet;
}