#include "vcpch.hpp"
#include "net/ServerLayer.hpp"
#include "net/packet/ConnectPacket.hpp"

#include "common/event/NetEvent.hpp"

void ServerLayer::OnUpdate()
{
    Packet packet;
    Address sender;

    while (m_socket.receive(packet, sender.ip, sender.port) != sf::Socket::NotReady)
    {
        auto type = VerifyPacket(packet);
        if (type != PacketType::Unrelated)
        {
            const User* user = GetUser(sender);

            switch (type)
            {
            case PacketType::Connect:
            {
            // First contact with this address
            //if (!user)
                if (type != PacketType::Connect)
                {
                    std::cout << sender << " tried to connect with the invalid packet: " << static_cast<int>(type) << ". How is this possible?\n";
                }

                // Add user
                {
                    auto request = ExtractPacket<ConnectPacket>(packet);

                    User newUser;
                    newUser.address = sender;
                    newUser.name = request.GetName();
                    m_users.push_back(newUser);

                    std::cout << "First contact from " << newUser.name << '(' << newUser.address << ")\n";
                }

                // Send handshake
                {
                    Send(m_users.back(), ConnectResponsePacket(ConnectResponsePacket::Status::Accepted).Build());
                    Publish(NetJoinedEvent(m_users.back()));
                }
            }
            break;
        
            case PacketType::Disconnect:
                std::cout << "DISCONNECT EVENT NOT HANDLED YET. Bye " + user->name << '\n';
                break;
            
            case PacketType::Gameplay:
            {
                Publish(NetServerPacketReceiveEvent(packet,*user));
            }
            break;
            
            default:
                std::cout << "Received unhandled packet type " << (int)type << '\n';
                break;
            }   
        }
    }
}

void ServerLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    // Connection events
    d.Dispatch<NetHostEvent>([&](NetHostEvent& e)
        {
            assert(!m_isHosting && "Server is already hosting");
            auto& config = e.config;
            m_config = config;

            if (m_socket.bind(config.address.port, config.address.ip) != sf::Socket::Done)
            {
                std::cout << "Could not bind the server to " << config.address << "\n Aborting server...\n";
                Publish(NetHostResponseEvent(NetHostResponseEvent::Status::Failed));
                return;// false;
            }

            m_isHosting = true;
            std::cout << "Hosting server at " << config.address << '\n';
            Publish(NetHostResponseEvent(NetHostResponseEvent::Status::Success));
        });
    d.Dispatch<NetShutdownEvent>([&](NetShutdownEvent&)
        {
            std::cout << "Shutting down server\n";
            SendAll(ShutdownPacket().Build());
            m_isHosting = false;
        });

    d.Dispatch<NetServerPacketSendEvent>([&](NetServerPacketSendEvent& e)
        {
            SendAll(e.packet);
        });
    //d.Dispatch<NetPacketReceiveEvent>([&](NetPacketSendEvent& e)
    //    {
    //        SendAll(e.packet);
    //    });
}

ServerLayer::ServerLayer()
    : m_isHosting(false)
{
    m_socket.setBlocking(false);
}

void ServerLayer::Send(User& user, const Packet& packet)
{
    auto pack = packet;
    m_socket.send(pack, user.address.ip, user.address.port);
}

void ServerLayer::SendAllBut(User& user, const Packet& packet)
{
    // Build the packet
    auto pack = packet;
    for (const auto& u : m_users)
    {
        if(u.name != user.name)
        if (m_socket.send(pack, u.address.ip, u.address.port) != sf::Socket::Done)
        {
            std::cout << "There was an issue sending data to " << u.name << '\n';
        }
    }
}

void ServerLayer::SendAll(const Packet& packet)
{
    // Build the packet
    auto pack = packet;
    for (const auto& user : m_users)
    {
        // NOTE: this function takes a reference to the packet
        // but it does not modify the packet. For some reason, 
        // the virtual functions are not const as well tho they 
        // don't modify anything in the source code
        // (for the udp implementation that is)
        // So I don't think that I have to be worried about it
        if (m_socket.send(pack, user.address.ip, user.address.port) != sf::Socket::Done)
        {
            std::cout << "There was an issue sending data to " << user.name << '\n';
        }
    }
}

const ServerLayer::User* ServerLayer::GetUser(const Address& address) const
{
    for (const auto& user : m_users)
        if (user.address == address)
            return &user;
    return nullptr;
}