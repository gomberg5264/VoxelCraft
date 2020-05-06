#include "Server.h"

void Server::Host(unsigned port)
{
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 25565;

    m_host = enet_host_create(&address, 32, 1, 0, 0);

    if (m_host == nullptr)
    {
        SH_ERROR("An error occurred while trying to create an ENet server host");
    }
}

void Server::Broadcast(ENetPacket* packet)
{
    enet_host_broadcast(m_host, 0, packet);
}

int Server::Poll(ENetEvent& event)
{
    return enet_host_service(m_host,&event,0);
}
