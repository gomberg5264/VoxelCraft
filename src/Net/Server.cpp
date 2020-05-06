#include "Server.h"

Server::Server()
    : m_host(nullptr)
{
}

bool Server::IsHosting() const
{
    return m_host;
}

bool Server::Host(unsigned port)
{
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 25565;

    m_host = enet_host_create(&address, 32, 1, 0, 0);

    if (m_host == nullptr)
    {
        SH_ERROR("An error occurred while trying to create an ENet server host");
        return false;
    }

    SH_INFO("Hosting! Listening on {0}:{1}", address.host, port);
    return true;
}

void Server::Close()
{
    // TODO: Gracefully disconnect clients
    SH_WARN("Server forcibly shut down!");
    enet_host_destroy(m_host);
}

void Server::Broadcast(ENetPacket* packet)
{
    enet_host_broadcast(m_host, 0, packet);
}

int Server::Poll(ENetEvent& event)
{
    return enet_host_service(m_host,&event,0);
}
