#pragma once
#include "Common/World.h"

#include <Shinobu/Common.h>
#include <Net/Server.h>

class ServerLayer : public sh::Layer
{
public:
    virtual void OnAttach() override
    {
        m_server.SetConnectCB([](Server& server, ENetAddress user)
            {
                SH_TRACE("Client connected from {0}:{1}", user.host, user.port);
                static unsigned unique = 0;
                JoinResponse response;
                response.userID = unique++;

                server.Broadcast(response);
            });

        m_server.SetDisconnectCB([](Server& server, ENetAddress user)
            {
                SH_TRACE("Client disconnected from {0}:{1}", user.host, user.port);
            });

        m_server.Host(25565);
    }
    //virtual void OnDetach() override;

    virtual void OnEvent(sh::Event& event) override
    {
        sh::EventDispatcher d(event);
        d.Dispatch<sh::KeyPressedEvent>([&](sh::KeyPressedEvent& e)
            {
                if (e.GetKeyCode() == sh::KeyCode::Escape)
                    sh::Application::Get().Exit();
                return false;
            });
    }
    virtual void OnUpdate(sh::Timestep ts) override final
    {
        std::shared_ptr<Packet> packet;
        while (m_server.Poll(packet))
        {
            switch (packet->GetType())
            {
            }
        }
    }

private:
    World m_world;
    Server m_server;
};