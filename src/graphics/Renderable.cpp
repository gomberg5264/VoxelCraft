#include "vcpch.hpp"

const glm::mat4& Renderable::GetToWorld() const
{
    return m_toWorld;
}

void Cube::LoadVBO()
{
    // Define a 3D cube (6 facesmade of 2 triangles composed by 3 vertices)
    static constexpr GLfloat cube[] =
    {
        // positions    // colors (r, g, b, a)
        -0.5f, -0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f, -0.5f,  0.5f,  0, 0, 1, 1,
        -0.5f, -0.5f,  0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 0, 1, 1,

         0.5f, -0.5f, -0.5f,  0, 1, 0, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  0, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  0, 1, 0, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 0, 1,
         0.5f,  0.5f,  0.5f,  0, 1, 0, 1,

        -0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
        -0.5f, -0.5f,  0.5f,  1, 0, 0, 1,
        -0.5f, -0.5f,  0.5f,  1, 0, 0, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 0, 0, 1,

        -0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 1, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 1, 1, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
         0.5f,  0.5f,  0.5f,  0, 1, 1, 1,

        -0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  1, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  1, 0, 1, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
         0.5f,  0.5f, -0.5f,  1, 0, 1, 1,

        -0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
        -0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
        -0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
         0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

unsigned Cube::VertexCount() const
{
    return 36;
}