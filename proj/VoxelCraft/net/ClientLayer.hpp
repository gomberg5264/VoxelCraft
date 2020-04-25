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
    virtual void OnUpdate() override final;

private:
    void Connect(NetConnectEvent& event);
    void Disconnect();

    sf::Socket::Status Send(Packet packet);
    /**
     * Helper function to receive a packet
     * It verifies if the packet is from the server
     * an returns true if there is a packet that is received
     */
    bool Receive(Packet& packet, PacketType& type);

    bool m_connected;
    
    Address m_server;
    std::string m_name;
    sf::UdpSocket m_socket;
};