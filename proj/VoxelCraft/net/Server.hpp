#pragma once
#include "net/packet/Packet.hpp"
#include "utils/Observer.hpp"
// TODO: Change to network event
#include "common/event/Event.hpp"


#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <vector>

/**
 * The server will handle packets and forward them to the application
 */
class Server
{
public:
    struct Address
    {
        sf::IpAddress ip;
        unsigned short port;
    };

    struct Config
    {
        Address address;
    };

    struct User
    {
        std::string name;
        Address address;
    };

    Server();

    /**
     * Host the server with the given config
     * Make sure that the engine has been constructed before
     * (which should happen automatically)
     */
    bool Host(Config config);
    void Close();

    void PollEvents(Publisher<Event>& event);

private:
    void Send(DataPacket&& data);
    const User* GetUser(const Address& address) const;

    bool m_isHosting;
    sf::UdpSocket m_socket;
    Config m_config;
    
    std::vector<User> m_users;
};

std::ostream& operator<<(std::ostream& os, const Server::Address& address)
{
    os << address.ip.toString() << ':' << address.port;
    return os;
}

bool operator==(const Server::Address& lhs, const Server::Address& rhs)
{
    return lhs.ip == rhs.ip && lhs.port == rhs.port;
}