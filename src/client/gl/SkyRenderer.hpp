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
    void SetCameraRotateProject(const glm::mat4& rotProj);
    void SetTime(float t);
    void Display();

private:
    float m_time;

    VAO m_vao;
    Shader m_shader;
    TextureAtlas m_texture;
};