#pragma once
#include "Common/Player.h"

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

    sh::TextureRef m_texture;
    std::vector<Player> m_players;
};