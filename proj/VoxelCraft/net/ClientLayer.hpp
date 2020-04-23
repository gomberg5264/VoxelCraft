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

    /**
     * Attempts an connection
     */
    //bool Connect(Address serverAddres, const char* name);
    //virtual void OnInit() override final;
    virtual void OnNotify(Event& event) override final;

private:
    void Connect(NetConnectEvent& event);
    
    sf::UdpSocket m_socket;
};