#pragma once
#include <Shinobu/Common.h>

#include <glm/gtc/matrix_transform.hpp>

class GameLayer : public sh::Layer
{
public:
    GameLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override { SH_INFO("Detached {0}", GetName()); }

    virtual void OnEvent(sh::Event& event) override;
    virtual void OnUpdate(sh::Timestep ts) override final;
    virtual void OnGuiRender() override final;

private:
    sh::PerspectiveCameraController m_camera;

    sh::Transform m_transform;
    std::shared_ptr<sh::VertexArray> m_cube;
    std::shared_ptr<sh::Shader> m_shader;

    std::shared_ptr<sh::Texture2D> m_texture;
};