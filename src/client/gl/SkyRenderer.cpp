#include "vcpch.hpp"
#include "SkyRenderer.hpp"

#include "client/gl/Primitive.hpp"
#include "utils/Math.hpp"

#include "glm/gtx/gradient_paint.hpp"

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
    // X is positive since sun rises from east
    // We could make this generic if we define in the transform class which axis represents which axis (fe x = east)
    auto sun = Primitive::Face::MakeBuffer(BlockFace::Left, distance, 0, 0 , 0).vertices;
    auto moon = Primitive::Face::MakeBuffer(BlockFace::Right, -distance, 0,  0, 1).vertices;
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

void SkyRenderer::SetCameraRotateProject(const glm::mat4& rotProj) noexcept
{
    m_shader.Bind();

    //const float angle = Math::InverseLerp(
    //    glm::sin(m_time * glm::pi<float>() * 2.f),-1.f,1.f) * 
    const auto rot = glm::rotate(m_time * glm::pi<float>() * 2.f, glm::vec3(0, 0, 1));
    m_shader.SetMatrix("aRotProj", glm::value_ptr(rotProj * rot));
}

void SkyRenderer::SetTime(float t) noexcept
{
    m_time = t;
}

glm::vec3 SkyRenderer::GetLightDir() const noexcept
{
    // 0 - 0.5 day
    // 0.25 afternoon
    // 0.5 - 1 night
    // 0.75 midnight

    // 0.25 light dir = -1
    // 0.75 ligt dir = 1

    // Sun dir east to west
    // positive x to negtive x
    // Although we could use transform directions to make this generalized
    
    // First we calculate the position of the sun
    const float period = m_time * glm::two_pi<float>();
    const float x = glm::cos(period);
    const float y = glm::sin(period);
    
    glm::vec3 lightDir = glm::vec3(0) - glm::vec3(x, y, 0);
    assert(glm::distance(lightDir) == 1 && "light dir is not normalized but should be, check x and y values");

    // If y is negative it means that moon is rising so we flip the direction
    if (y <= 0.f) lightDir *= -1.f;
    //std::cout << lightDir.x << '\n';
    //std::cout << glm::dot(-lightDir, glm::vec3(1, 0, 0)) << '\n';
    return lightDir;
}

float SkyRenderer::GetSkyAmbient() const noexcept
{
    const float period = m_time * glm::two_pi<float>();

    // I'm not too sure how ambient lighting intensity should be calculated, so I'll take the 
    // sin of the time and map that to a certain range
    // TODO: Research or test correct ambient surface lighting values

    float intensity = glm::sin(period);
    constexpr float max = 0.8f;
    constexpr float min = 0.3f;

    return Math::Map(intensity,-1.f,1.f,min,max);
}

float SkyRenderer::GetIntensity() const noexcept
{ 
    const float period = m_time * glm::two_pi<float>();
    // TODO: Right now I change the strength based on the sin
    // but the intensity should be a constant
    // We may want to change that

    float val = glm::sin(period);
    if (val < 0.f)
        val *= -0.3f;
    return val;
}

glm::vec3 SkyRenderer::GetColor() const noexcept
{
    return glm::vec3(0.9f);
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