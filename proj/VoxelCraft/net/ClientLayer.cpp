#include "vcpch.hpp"

#include "net/ClientLayer.hpp"
#include "packet/ConnectPacket.hpp"

ClientLayer::ClientLayer()
{
    m_socket.setBlocking(false);
}

void ClientLayer::Connect(NetConnectEvent& event)
{
    auto server = event.server;
    auto name = event.name;
    
    auto packet = JoinRequestPacket(name).Build();
    m_socket.bind(sf::Socket::AnyPort);
    if (m_socket.send(packet, server.ip, server.port) != sf::Socket::Done)
    {
        std::cout << "Can't send package, check internet connection\n";
        return;// false;
    }

    m_socket.setBlocking(true);
    // TODO: Make this non block or on another thread

    Address sender;
    if (m_socket.receive(packet, sender.ip, sender.port) != sf::Socket::Done)
    {
        std::cout << "Failed to receive packet\n";
        return;// false;
    }

    auto type = VerifyPacket(packet);
    std::cout << "Received packet type :" << static_cast<int>(type) << '\n';
    return;// true;
}

void ClientLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    d.Dispatch<NetConnectEvent>(BIND(ClientLayer::Connect));
    //d.Dispatch<NetDisconnectEvent>()


    //d.Dispatch<NetReceivePacketEvent>([](NetReceivePacketEvent& e)
    //    {

    //    });
}
