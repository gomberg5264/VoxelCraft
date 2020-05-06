#include "Client.h"

Client::Client()
{
    m_host = enet_host_create(nullptr, 1, 1, 0, 0);
    if (m_host == nullptr)
    {
        SH_ERROR("An error occurred while trying to create an ENet client host");
    }
}

void Client::Connect(const std::string& host, unsigned port)
{
    ENetAddress address;
    ENetEvent event;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    m_client = enet_host_connect(m_host, &address, 1, 0);
    if (m_client == nullptr)
    {
        SH_ERROR("No available peers for initiating an ENet connection");
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(m_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        SH_INFO("Successfully connected with {0}:{1}", host, port);
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(m_client);
        SH_ERROR("Failed to connect with {0}:{1}", host, port);
    }
}

void Client::SendPacket(ENetPacket* packet)
{
    enet_peer_send(m_client, 0, packet);

    // We may want to let Poll handle this 
    // because now we send events every time this is called
    enet_host_flush(m_host);
}

int Client::Poll(ENetEvent& event)
{
    return enet_host_service(m_host, &event, 0);
}
