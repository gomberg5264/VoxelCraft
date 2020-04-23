#pragma once
#include "net/Address.hpp"
#include "common/event/NetEvent.hpp"

#include <SFML/Network/UdpSocket.hpp>

/**
 * The client class abstracts a connection to a server
 * It will forward packets to the engine
 */

class Client
{
public:
    Client();

    /**
     * Attempts an connection
     */
    bool Connect(Address serverAddres, const char* name);
    void HandleEvents(Event& event);

private:
    sf::UdpSocket m_socket;
};