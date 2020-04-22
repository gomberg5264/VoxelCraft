#include "vcpch.hpp"

#include "net/Client.hpp"
#include "packet/ConnectPacket.hpp"

Client::Client()
{
    m_socket.setBlocking(false);
}

bool Client::Connect(Address serverAddres, const char* name)
{
    auto packet = JoinRequestPacket(name).Build();
    m_socket.bind(sf::Socket::AnyPort);
    if (m_socket.send(packet, serverAddres.ip, serverAddres.port) != sf::Socket::Done)
    {
        std::cout << "Can't send package, check internet connection\n";
        return false;
    }

    m_socket.setBlocking(true);
    // TODO: Make this non block or on another thread

    Address sender;
    if (m_socket.receive(packet, sender.ip, sender.port) != sf::Socket::Done)
    {
        std::cout << "Failed to receive packet\n";
        return false;
    }

    auto type = VerifyPacket(packet);
    std::cout << "Received packet type :" << static_cast<int>(type) << '\n';
    return true;
}
