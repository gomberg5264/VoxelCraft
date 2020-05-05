#pragma once

#include "Common/Command.h"

#include <Shinobu/Core/Transform.h>
#include <Shinobu/Renderer/VertexArray.h>
#include <Shinobu/Renderer/VertexBuffer.h>
#include <Shinobu/Renderer/Shader.h>

class Player
{
public:
    Player(/*unsigned id*/);

    void Draw() const;

public:
    //const unsigned id;
    
    sh::Transform transform;

private:
    sh::ShaderRef m_shader;
    sh::VertexArrayRef m_mesh;
};