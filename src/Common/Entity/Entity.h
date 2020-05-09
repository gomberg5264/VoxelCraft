#pragma once

#include <Shinobu/Core/Core.h>
#include <Shinobu/Core/Transform.h>
#include <Shinobu/Renderer/VertexArray.h>
#include <Shinobu/Renderer/VertexBuffer.h>
#include <Shinobu/Renderer/Shader.h>

// TODO: Add cereal to engine so that we can serialze transform there
#include "Net/Packet.h"
namespace sh
{

template<class Archive>
void save(Archive& archive,
    Transform const& m)
{
    auto pos = m.GetPosition();
    auto rot = m.GetEulerRotation();
    archive(pos.x, pos.y, pos.z);
    archive(rot.x, rot.y, rot.z);
}

template<class Archive>
void load(Archive& archive,
    Transform& m)
{
    auto pos = m.GetPosition();
    auto rot = m.GetEulerRotation();
    archive(pos.x, pos.y, pos.z);
    archive(rot.x, rot.y, rot.z);
    m.SetPosition(pos);
    m.SetRotation(rot);
}
}

/**
 * An entity is a moveable and damageable object in the world
 */
class Entity
{
public:
    Entity();

    void Draw() const;

    template <typename T>
    void serialize(T& archive)
    {
        archive(id);
        archive(transform);
    }

public:
    unsigned id;
    sh::Transform transform;

private:
    sh::ShaderRef m_shader;
    sh::VertexArrayRef m_mesh;
};
