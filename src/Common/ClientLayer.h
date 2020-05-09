#pragma once
#include "Common/World.h"
#include "Common/GamePacket.h"
#include "Common/Entity/Entity.h"
#include "Common/Entity/EntityCommand.h"
#include "Net/Client.h"

#include <Shinobu/Common.h>

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
            switch (packet->GetType())
            {
            case PacketType::JoinResponse:
                SH_INFO("Joined with ID {0}", static_cast<JoinResponsePacket*>(packet.get())->userID);
                break;
            case PacketType::EntityCommand:
            {
                auto* cast = static_cast<EntityCommandPacket*>(packet.get());

                cast->command->Execute(m_world.entities);
                
                SH_INFO("Player count {0}", m_world.entities.size());

                //SH_INFO("Joined with ID {0}", static_cast<JoinResponsePacket*>(packet.get())->userID);
                break;
            }

            }
        }
    }

private:
    World m_world;
    Client m_client;
};