#pragma once
#include "client/gl/Buffer.hpp"
#include "client/gl/Shader.hpp"
#include "client/gl/Texture.hpp"

/**
 * Responsible for rendering the skybox color,
 * sun and moon position. Everything that is in the sky
 * and should not translate
 *
 * TODO: Make a texture manager so that we can bind multiple textures
 */
class SkyRenderer
{
public:
    SkyRenderer() noexcept;

    /**
     * @param t [0,1] where [0,0.5] is day and (0.5,1] is night
     */
    void SetTime(float t) noexcept;
    void SetCameraRotateProject(const glm::mat4& rotProj) noexcept;

    glm::vec3 GetLightDir() const noexcept;
    /**
     * @return get the sky intensity which is used for the ambient product
     */
    float GetSkyAmbient() const noexcept;
    /**
     * The intensity of the emitted light
     */
    float GetIntensity() const noexcept;
    glm::vec3 GetColor() const noexcept;

    void Display();

private:
    void LoadStars();
    void LoadSkybox();

    float m_time;

    VAO m_starVAO;
    Shader m_starShader;
    TextureAtlas m_starTex;

    VAO m_skyVAO;
    Shader m_skyShader;
    TextureCubemap m_skyTex;
};