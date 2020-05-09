#include "Client.h"

#include <Shinobu/Core/Timer.h>

Client::Client()
    : m_host(enet_host_create(nullptr, 1, 1, 0, 0))
    , m_client(nullptr)
{
    if (m_host == nullptr)
    {
        SH_ERROR("An error occurred while trying to create an ENet client host");
    }
}

Client::~Client()
{
    if(IsConnected()) Disconnect();
}


bool Client::IsConnected() const
{
    return m_host->connectedPeers == 1;
}

bool Client::Connect(const std::string& host, unsigned port)
{
    ENetAddress address;
    ENetEvent event;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    m_client = enet_host_connect(m_host, &address, 1, 0);
    if (m_client == nullptr)
    {
        SH_ERROR("No available peers for initiating an ENet connection");
        return false;
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    SH_TRACE("Attempting connection with {0}:{1}", address.host, port);
    if (enet_host_service(m_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        SH_INFO("Successfully connected with {0}:{1}", address.host, port);
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(m_client);
        SH_ERROR("Failed to connect with {0}:{1}", address.host, port);
        return false;
    }
    return true;
}

void Client::Disconnect()
{
    SH_ASSERT(IsConnected(), "Client can't disconnect if they are not connected!");

    // Attempt graceful disconnect
    enet_peer_disconnect(m_client, 0);

    sh::Timer timer;

    while (timer.Seconds() < 5.f)
    {
        ENetEvent event;
        while (enet_host_service(m_host, &event, 0) > 0)
        {
            if (event.type == ENET_EVENT_TYPE_RECEIVE)
                enet_packet_destroy(event.packet);
            else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
            {
                SH_TRACE("Gracefully disconnected from server");
                return;
            }
        }
    }
    
    SH_WARN("Forcefully disconnected form the server");
    enet_peer_reset(m_client);
}

void Client::SendPacket(Packet& packet)
{
    // Serialze the archive to binary
    auto stream = PacketToBinary(packet);

    stream.seekg(0, std::ios::end);
    ENetPacket* pck = enet_packet_create(stream.rdbuf(), stream.tellg(),ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(m_client, 0, pck);
    enet_host_flush(m_host);
}

bool Client::Poll(std::shared_ptr<Packet>& packet)
{
    ENetEvent event;

    int status = enet_host_service(m_host, &event, 0);
    SH_ASSERT(status >= 0, "Client returned with error");

    if (status == 0)
        return false;

    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
        SH_TRACE("Connected to {0}:{1}", event.peer->address.host, event.peer->address.port);
        // We do this since connecting and disconnecting don't actually return a packet
        return Poll(packet); 
        break;
    case ENET_EVENT_TYPE_DISCONNECT:
        SH_TRACE("Disconnected from {0}:{1}", event.peer->address.host, event.peer->address.port);
        return Poll(packet);
        break;
    case ENET_EVENT_TYPE_RECEIVE:
        SH_TRACE("Retrieved packet from {0}:{1}", event.peer->address.host, event.peer->address.port);
        enet_packet_destroy(event.packet);
        // TODO: Create a package and set it

        //SH_TRACE("Received packet from server {0}:{1}", event.peer->address.host, event.peer->address.port);

        // Steps...
        // Extract command
        // Apply command

        // Extract command
        //sf::Packet data;
        //for (uint8_t i = 0; i < event.packet->dataLength; i++)
        //{
        //    data << event.packet->data[i];
        //}
        //unsigned type;
        //data >> type;

        //switch (type)
        //{
        //case COMMAND_TYPE_JOIN:
        //{

        //    unsigned playerID;
        //    data >> playerID;
        //    m_input = std::make_unique<PlayerInput>(playerID);
        //    m_input->callback = [](std::unique_ptr<Command>&& command)
        //    {
        //        m_commands.emplace_back(std::move(command));
        //        //command->Execute();
        //    };
        //}
        //break;
        //case COMMAND_TYPE_MOVE:
        //{

        //    unsigned playerID;
        //    glm::vec3 pos;
        //    glm::vec3 oldPos;
        //    data >> playerID >> pos >> oldPos;
        //    MoveCommand command(m_players[playerID], pos);

        //    // Verify command
        //    // Apply command
        //    command.Execute();
        //}
        //break;

        //}

        //enet_packet_destroy(event.packet);
        //break;
    }
    return true;
}
