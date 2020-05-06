#pragma once

#include <enet/enet.h>

class Server
{
public:
    Server();

    bool IsHosting() const;

    bool Host(unsigned port);
    void Close();

    void Broadcast(ENetPacket* packet);
    int Poll(ENetEvent& event);

private:
    ENetHost* m_host;
};