#pragma once
#include "common/Player.hpp"
#include "client/Camera.hpp"

#include "client/gl/Buffer.hpp"
#include "client/gl/Primitive.hpp"
#include "client/gl/Shader.hpp"

class PlayerMesh
{
public:
    PlayerMesh(Transform& transform);

    void Draw(const Camera& camera);

private:
    Shader m_shader;

    Transform& m_transform;
    VAO m_vao;
};