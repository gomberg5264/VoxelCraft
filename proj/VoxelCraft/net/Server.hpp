#pragma once
#include "net/packet/Packet.hpp"
#include "net/Address.hpp"
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

    /**
     * Handles all UDP packages that have arrived.
     * Sends events to the publisher 
     */
    void PollEvents(Publisher<Event>& publisher);

private:
    void Send(User& user, const PacketData& data);
    void SendAll(PacketData&& data);
    const User* GetUser(const Address& address) const;

    bool m_isHosting;
    sf::UdpSocket m_socket;
    Config m_config;
    
    std::vector<User> m_users;
};