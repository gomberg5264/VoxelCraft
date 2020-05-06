#pragma once
#include "Client/PlayerInput.h"
#include "Common/Player.h"

#include <Shinobu/Common.h>
#include <Net/Client.h>
#include <Net/Server.h>

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <future>

class GameLayer : public sh::Layer
{
public:
    GameLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(sh::Event& event) override;
    virtual void OnUpdate(sh::Timestep ts) override final;
    virtual void OnGuiRender() override final;

public:
    // TODO: I'm not sure if static is the way to go
    // But we need some way to access this via commands
    // since they need to execute logic on the player vector
    //
    // We could pass the vector in the command constructor
    // or add a callback that the input calls to push it commands 
    // to, but this is simplest for now.
    static std::vector<Player> m_players;

private:
    sh::PerspectiveCamera m_camera;

    sh::TextureRef m_texture;

    PlayerInput m_input;

    Client m_client;
    Server m_server;

    std::thread m_serverThread;
};