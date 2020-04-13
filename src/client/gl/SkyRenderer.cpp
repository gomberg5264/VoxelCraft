#include "vcpch.hpp"
#include "SkyRenderer.hpp"

#include "client/gl/Primitive.hpp"
#include "utils/Math.hpp"

SkyRenderer::SkyRenderer() noexcept
    : m_time(0.f)
    , m_vao({{ 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
             { 1, 3, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
             { 2, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 6u * sizeof(float) },
             { 3, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 8u * sizeof(float) }})
    , m_shader("res/shaders/sky.vert", "res/shaders/sky.frag")
    , m_texture("res/sunMoon.png",2,1)
{
    m_shader.Bind();
     
    m_vao.Bind();
    // Fill VBO
    constexpr float distance = 10.f;
    auto sun = Primitive::Face::MakeBuffer(BlockFace::Right, -distance, 0, 0 , 0).vertices;
    auto moon = Primitive::Face::MakeBuffer(BlockFace::Left, distance, 0,  0, 1).vertices;
    sun.insert(std::end(sun), std::begin(moon), std::end(moon));
    m_vao.m_vbo.Bind();
    m_vao.m_vbo.Upload(sun);
    m_vao.m_vbo.Unbind();

    // Fill EBO
    m_vao.m_ebo = std::make_unique<EBO>();
    m_vao.m_ebo->Bind();
    m_vao.m_ebo->Upload(Primitive::Face::MakeIndices(2));    
    
    m_vao.Unbind();
}

void SkyRenderer::SetCameraRotateProject(const glm::mat4& rotProj)
{
    m_shader.Bind();

    //const float angle = Math::InverseLerp(
    //    glm::sin(m_time * glm::pi<float>() * 2.f),-1.f,1.f) * 
    const auto rot = glm::rotate(m_time * glm::pi<float>() * 2.f, glm::vec3(0, 0, 1));
    m_shader.SetMatrix("aRotProj", glm::value_ptr(rotProj * rot));
}

void SkyRenderer::SetTime(float t)
{
    m_time = t;
}

void SkyRenderer::Display()
{
    glDepthMask(GL_FALSE);
    //glDepthFunc(GL_LESS);
    m_shader.Bind();
    m_texture.Bind();
    m_vao.Bind();
    glDrawElements(GL_TRIANGLES, m_vao.m_ebo->ElementCount(), GL_UNSIGNED_INT, 0);
    m_vao.Unbind();
    glDepthMask(GL_TRUE);
}