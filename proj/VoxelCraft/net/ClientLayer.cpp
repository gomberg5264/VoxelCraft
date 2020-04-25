#include "vcpch.hpp"

#include "net/ClientLayer.hpp"
#include "net/packet/ConnectPacket.hpp"

ClientLayer::ClientLayer()
    : m_connected(false)
{
    m_socket.setBlocking(false);
}

void ClientLayer::Connect(NetConnectEvent& event)
{
    assert(!m_connected && "Client is already connected");

    m_server = event.server;
    m_name = event.name;

    // Create a port to receive messages on
    if (m_socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cout << "Can't bind to a socket. There may be a problem with your drivers or the port is already bound\n"; return;
        Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
    }

    std::cout << "Attempting connection to " << m_server << '\n';

    // Wait for server to return handshake
    Address sender;
    Packet packet;
    {
        bool responded = false;
        float et = 0;
        int count = 0;
        Timer time;
        std::cout << "seconds: ";
        
        if (Send(ConnectPacket(m_name.c_str())) != sf::Socket::Done)
        {
            std::cout << "Can't send package, check internet connection\n";
            Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
            return;
        }
        
        while (!responded)
        {
            time.Update();
            et += time.Elapsed();

            if (m_socket.receive(packet, sender.ip, sender.port) == sf::Socket::Done)
            {
                // Verify sender
                responded = (sender == m_server);
            }

            // Print current time
            if (et > 1.f)
            {
                // Send handshake to the given server
                if (Send(ConnectPacket(m_name.c_str())) != sf::Socket::Done)
                {
                    std::cout << "Can't send package, check internet connection\n";
                    Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
                    return;
                }
                et -= 1.f;
                count++;
                std::cout << count << ' ';
            }

            if (count >= 10)
                break;

        }

        std::cout << '\n';

        if (!responded)
        {
            std::cout << "Timed out. No handshake response, the server may be offline\n"; 
            Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
            return;
        }
    }

    // Verify if correct handshake type 
    auto type = VerifyPacket(packet);
    ConnectResponsePacket response = ExtractPacket<ConnectResponsePacket>(packet);
    if (response.type != PacketType::ConnectResponse)
    {
        std::cout << "Retrieved a handshake response but it is the wrong type???, scream at the programmer\n"; 
        Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
        return;
    }

    // Respond to server response
    if (response.GetStatus() != ConnectResponsePacket::Status::Accepted)
    {
        switch (response.GetStatus())
        {
        case ConnectResponsePacket::Status::Full:
            std::cout << "The server is full\n";
            Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
            return;
        default:
            std::cout << "Handshake returned unknown status: " << static_cast<int>(response.GetStatus()) << ". Complain to programmer\n";
            Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Failed));
            return;
        }
    }

    std::cout << "Joined server " << sender << '\n';

    m_connected = true;
    Publish(NetConnectResponseEvent(NetConnectResponseEvent::Status::Success));
}

void ClientLayer::Disconnect()
{
    m_socket.unbind();
    m_connected = false;
}

sf::Socket::Status ClientLayer::Send(const PacketData& data)
{
    //assert(m_connected && "Client isn't connected to a server");
    auto p = data.Build();
    return m_socket.send(p, m_server.ip, m_server.port);
}

bool ClientLayer::Receive(Packet& packet, PacketType& type)
{
    assert(m_connected && "Client is not connected");
    Address sender;
    if (m_socket.receive(packet, sender.ip, sender.port) == sf::Socket::Done)
    {
        assert(sender == m_server);
        type = VerifyPacket(packet);
        return type != PacketType::Unrelated;
    }
    return false;
}

void ClientLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    // Connection events
    d.Dispatch<NetConnectEvent>(BIND(ClientLayer::Connect));
    d.Dispatch<NetDisconnectEvent>([&](Event&)
        {   
            Send(DisconnectPacket());
            m_socket.unbind();
        });
    //// Gameplay events
    //d.Dispatch<NetMessageEvent>([&](NetMessageEvent& e)
    //    {
    //        Send(MessagePacket(e.message));
    //    });
}

void ClientLayer::OnUpdate()
{
    Packet packet;
    PacketType type;
    while (Receive(packet, type))
    {
        switch (type)
        {
        //case PacketType::Unrelated:
        //    break;
        //case PacketType::Connect:
        //    break;
        //case PacketType::ConnectResponse:
        //    break;
        //case PacketType::Disconnect:
        //    break;
        //case PacketType::Message:
        //{
        //    MessagePacket p = ExtractPacket<MessagePacket>(packet);
        //    std::cout << p.message << '\n';
        //}
        //break;
        case PacketType::Shutdown:
            std::cout << "Server has shut down\n";
            Disconnect();

            break;
        default:
            break;
        }
    }
}
