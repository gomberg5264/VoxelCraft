#include "Server.h"

Server::Server()
    : m_host(nullptr)
    , m_connectCB(nullptr)
    , m_disconnectCB(nullptr)
{
}

Server::~Server()
{
    if (IsHosting()) Close();
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
    SH_ASSERT(IsHosting(), "Can't shutdown a server that is not hosting");

    // TODO: Gracefully disconnect clients
    SH_WARN("Server forcibly shut down!");
    enet_host_destroy(m_host);
}

void Server::Broadcast(Packet& packet)
{
    auto stream = PacketToBinary(packet);

    stream.seekg(0, std::ios::end);
    ENetPacket* pck = enet_packet_create(stream.rdbuf(), stream.tellg(), ENET_PACKET_FLAG_RELIABLE);

    enet_host_broadcast(m_host, 0, pck);
    enet_host_flush(m_host);
}

bool Server::Poll(std::shared_ptr<Packet>& packet)
{
    ENetEvent event;

    int status =  enet_host_service(m_host,&event,0);
    SH_ASSERT(status >= 0, "Server returned with error");
    
    if (status == 0)
        return false;

    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
        if(m_connectCB) m_connectCB(*this, event.peer->address);
        return Poll(packet);
    }
    break;
    case ENET_EVENT_TYPE_DISCONNECT:
        if (m_disconnectCB) m_disconnectCB(*this, event.peer->address);
        return Poll(packet);
        break;

    case ENET_EVENT_TYPE_RECEIVE:
        SH_TRACE("Retrieved packet from {0}:{1}", event.peer->address.host, event.peer->address.port);
        enet_packet_destroy(event.packet);
        // TODO: Create a package and set it

        // Steps...
        // Extract command
        // Verify command
        // Apply command
        // Send command to all clients

        // Extract command
        //sf::Packet data;
        //for (uint8_t i = 0; i < event.packet->dataLength; i++)
        //{
        //    data << event.packet->data[i];
        //}
        //sf::Packet dataBack(data);
        ////event.packet->data
        //
        //unsigned playerID;
        //glm::vec3 pos;
        //glm::vec3 oldPos;
        //data >> playerID >> pos >> oldPos;
        //MoveCommand command(m_players[playerID], pos);
        //
        //
        //// Verify command
        //// Apply command
        //command.Execute();
        //
        //// Send command to all clients
        //auto* pck = enet_packet_create(dataBack.getData(), dataBack.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
        //m_server.Broadcast(pck);
        //
        //enet_packet_destroy(event.packet);
        //break;
    }
    return true;
}
