#pragma once

#include "Net/Packet.h"

#include <enet/enet.h>
#include <functional>
#include <string>

class Client
{
public:
    Client();
    ~Client();

    bool IsConnected() const;

    bool Connect(const std::string& host, unsigned port);
    void Disconnect();

    void SendPacket(Packet& packet);
    bool Poll(std::shared_ptr<Packet>& packet);

private:
    ENetHost* m_host;
    ENetPeer* m_client;
};