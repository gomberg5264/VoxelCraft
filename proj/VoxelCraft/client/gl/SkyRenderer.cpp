#include "vcpch.hpp"
#include "SkyRenderer.hpp"

#include "client/gl/Primitive.hpp"
#include "utils/Math.hpp"

#include "glm/gtx/gradient_paint.hpp"

void SkyRenderer::LoadStars()
{
    // Fill VBO
    constexpr float distance = 10.f;
    // X is positive since sun rises from east
    // We could make this generic if we define in the transform class which axis represents which axis (fe x = east)
    auto sun = Primitive::Face::MakeBuffer(BlockFace::Left, distance, 0, 0, 0).vertices;
    auto moon = Primitive::Face::MakeBuffer(BlockFace::Right, -distance, 0, 0, 1).vertices;
    sun.insert(std::end(sun), std::begin(moon), std::end(moon));
    
    m_starVAO.m_vbo.Upload(sun);

    // Fill EBO
    m_starVAO.MakeEBO()->Upload(Primitive::Face::MakeIndices(2));
    m_starVAO.Unbind();
}

void SkyRenderer::LoadSkybox()
{
    const std::vector<float> skyboxVertices 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    m_skyVAO.m_vbo.Upload(skyboxVertices);
}

SkyRenderer::SkyRenderer() noexcept
    : m_time(0.f)
    , m_starVAO(std::move(Primitive::Face::MakeVBO()))
    , m_starShader("res/shaders/star.vert", "res/shaders/star.frag")
    , m_starTex("res/sunMoon.png",2,1)
    , m_skyVAO(VBO({{ 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0 }}))
    , m_skyShader("res/shaders/sky.vert", "res/shaders/sky.frag")
    , m_skyTex("res/skybox/realistic","jpg")
{
    LoadStars();
    LoadSkybox();
}

void SkyRenderer::SetCameraRotateProject(const glm::mat4& rotProj) noexcept
{
    m_starShader.Bind();

    //const float angle = Math::InverseLerp(
    //    glm::sin(m_time * glm::pi<float>() * 2.f),-1.f,1.f) * 
    const auto rot = glm::rotate(m_time * glm::pi<float>() * 2.f, glm::vec3(0, 0, 1));
    m_starShader.SetMatrix("aRotProj", glm::value_ptr(rotProj * rot));

    m_skyShader.Bind();
    m_skyShader.SetMatrix("aRotProj", glm::value_ptr(rotProj));
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
    //assert(glm::length(lightDir) == rad  && "light dir is not normalized but should be, check x and y values");

    // If y is negative it means that moon is rising so we flip the direction
    if (y <= 0.f) lightDir *= -1.f;

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

    // Render skybox
    m_skyShader.Bind();
    m_skyTex.Bind();
    m_skyVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, m_skyVAO.m_ebo->ElementCount(), GL_UNSIGNED_INT, 0);

    // Render stars
    m_starShader.Bind();
    m_starTex.Bind();
    //m_skybox.Bind();
    m_starVAO.Bind();
    glDrawElements(GL_TRIANGLES, m_starVAO.m_ebo->ElementCount(), GL_UNSIGNED_INT, 0);
    
    m_starVAO.Unbind();
    glDepthMask(GL_TRUE);
}