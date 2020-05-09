#pragma once
#include "Common/World.h"

#include <Shinobu/Common.h>
#include <Net/Client.h>

class ClientLayer : public sh::Layer
{
public:
    virtual void OnAttach() override
    {
        while (!m_client.IsConnected())
        {
            m_client.Connect("localhost", 25565);
        }
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
        while (m_client.Poll(packet))
        {
            //switch (packet->GetType())
            //{
            //}
        }
    }

private:
    World m_world;
    Client m_client;
};