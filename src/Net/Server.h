#pragma once

#include "Net/Packet.h"

#include <enet/enet.h>
#include <functional>

class Server
{
public:
    using ConnectCB = std::function<void(Server&)>;

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

private:
    ENetHost* m_host;

    ConnectCB m_connectCB;
};