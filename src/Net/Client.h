#pragma once

#include <enet/enet.h>

#include <string>

class Client
{
public:
    Client();

    bool IsConnected() const;

    bool Connect(const std::string& host, unsigned port);
    void Disconnect();

    void SendPacket(ENetPacket* packet);
    int Poll(ENetEvent& event);

private:
    ENetHost* m_host;
    ENetPeer* m_client;
};