#pragma once
#include "Common/Player.h"

#include <Shinobu/Common.h>
#include <Net/Server.h>

class ServerLayer : public sh::Layer
{
public:
    ServerLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(sh::Event& event) override;
    virtual void OnUpdate(sh::Timestep ts) override final;

public:
    static std::vector<Player> m_players;

private:
    Server m_server;
};