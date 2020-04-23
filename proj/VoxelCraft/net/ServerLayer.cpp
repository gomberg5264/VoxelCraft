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

            // First contact with this address
            if (!user)
            {
                // Add user
                {
                    auto request = ExtractPacket<ConnectPacket>(packet);

                    User user;
                    user.address = sender;
                    user.name = request.GetName();
                    m_users.push_back(user);

                    std::cout << "First contact from " << user.name << '(' << user.address << ")\n";
                }

                // Send handshake
                {
                    auto joinRet = ConnectResponsePacket(ConnectResponsePacket::Status::Accepted);
                    Send(m_users.back(), joinRet);
                }
            }
            else
            {
                // TODO: handle the packets here
                std::cout << "Received data from " << user->address;
            }
        }
    }
}

void ServerLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    d.Dispatch<NetHostEvent>([&](NetHostEvent& e)
        {
            assert(m_isHosting && "Server is already hosting");
            auto& config = e.config;
            m_config = config;

            if (m_socket.bind(config.address.port, config.address.ip) != sf::Socket::Done)
            {
                std::cout << "Could not bind the server to " << config.address << "\n Aborting server...\n";
                return;// false;
            }

            m_isHosting = true;
            return;// true;
        });

    d.Dispatch<NetShutdownEvent>([&](NetShutdownEvent&)
        {
            SendAll(ShutdownPacket());
            m_isHosting = false;
        });
}

ServerLayer::ServerLayer()
    : m_isHosting(false)
{
    m_socket.setBlocking(false);
}

void ServerLayer::Send(User& user, const PacketData& data)
{
    auto packet = data.Build();
    m_socket.send(packet, user.address.ip, user.address.port);
}

void ServerLayer::SendAll(PacketData&& data)
{
    // Build the packet
    auto packet = data.Build();

    for (const auto& user : m_users)
    {
        // NOTE: this function takes a reference to the packet
        // but it does not modify the packet. For some reason, 
        // the virtual functions are not const as well tho they 
        // don't modify anything in the source code
        // (for the udp implementation that is)
        // So I don't think that I have to be worried about it
        if (m_socket.send(packet, user.address.ip, user.address.port) != sf::Socket::Done)
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