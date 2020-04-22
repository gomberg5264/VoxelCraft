#include "vcpch.hpp"
#include "net/Server.hpp"
#include "net/packet/ConnectPacket.hpp"

#include "common/Application.hpp"

Server::Server()
{
    m_socket.setBlocking(false);
}

bool Server::Host(Config config)
{
    assert(m_isHosting && "Server is already hosting");
    m_config = config;

    if (m_socket.bind(config.address.port, config.address.ip) != sf::Socket::Done)
    {
        std::cout << "Could not bind the server to " << config.address << "\n Aborting server...\n";
        return false;
    }

    m_isHosting = true;
    return true;
}

void Server::Close()
{
    Send(ShutdownPacket());
    m_isHosting = false;
}

void Server::Send(DataPacket&& data)
{
    // Build the packet
    auto packet = data.Build();

    for (const auto& user : m_users)
    {
        // NOTE: this function takes a reference to the packet
        // but it does not modify the packet. For some reason, 
        // the virtual functions are not const as well tho they 
        // don't modify anything in the source code
        // (for the udp implementation that is)
        // So I don't think that I have to be worried about it
        if (m_socket.send(packet, user.address.ip, user.address.port) != sf::Socket::Done)
        {
            std::cout << "There was an issue sending data to " << user.name << '\n';
        }
    }
}

const Server::User* Server::GetUser(const Address& address) const
{
    for (const auto& user : m_users)
        if (user.address == address)
            return &user;
    return nullptr;
}

void Server::PollEvents(Publisher<Event>& event)
{
    Packet packet;
    Address sender;
    
    while (m_socket.receive(packet, sender.ip, sender.port) != sf::Socket::NotReady)
    {
        const User* user = GetUser(sender);
        // First contact with this address
        if (!user)
        {
            // TODO: Handle handshake
            std::cout << "First contact from " << sender;
            User user;
            user.address = sender;
            m_users.push_back(user);
        }
        else
        {
            // TODO: handle the events here
            std::cout << "Received data from " << user->address;
        }
    }
}