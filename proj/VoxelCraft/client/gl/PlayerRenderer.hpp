#pragma once
#include "common/Player.hpp"
#include "client/Camera.hpp"

#include "client/gl/Buffer.hpp"
#include "client/gl/Primitive.hpp"
#include "client/gl/Shader.hpp"

class PlayerMesh
{
public:
    PlayerMesh(const Transform& transform);

    void Draw(const Camera& camera);

private:
    Shader m_shader;

    const Transform& m_transform;
    VAO m_vao;
};