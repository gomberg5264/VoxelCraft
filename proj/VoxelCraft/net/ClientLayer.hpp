#pragma once
#include "common/Application.hpp"
#include "common/event/NetEvent.hpp"

#include "net/Address.hpp"

#include <SFML/Network/UdpSocket.hpp>

/**
 * The client class abstracts a connection to a server
 * It will forward packets to the engine
 */

class ClientLayer : public Layer
{
public:
    ClientLayer();

    virtual void OnNotify(Event& event) override final;

private:
    void Connect(NetConnectEvent& event);
    sf::Socket::Status Send(const PacketData& data);

    bool m_connected;
    Address m_server;
    std::string m_name;
    sf::UdpSocket m_socket;
};