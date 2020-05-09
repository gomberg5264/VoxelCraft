#pragma once

#include "Net/Packet.h"

#include <enet/enet.h>
#include <functional>

class Server
{
public:
    // TODO: Change ENetAddress to an actual address instead of using enet
    using ConnectCB = std::function<void(Server&, ENetAddress user)>;

public:
    Server();
    ~Server();

    bool IsHosting() const;

    bool Host(unsigned port);
    void Close();

    void Broadcast(Packet& packet);
    // Returns true if a packet has been received
    bool Poll(std::shared_ptr<Packet>& packet);

    void SetConnectCB(ConnectCB cb) { m_connectCB = cb; }
    void SetDisconnectCB(ConnectCB cb) { m_disconnectCB = cb; }

private:
    ENetHost* m_host;

    ConnectCB m_connectCB;
    ConnectCB m_disconnectCB;
};