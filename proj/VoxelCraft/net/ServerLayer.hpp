#pragma once
#include "common/Application.hpp"

#include "net/Address.hpp"
#include "net/packet/Packet.hpp"

#include <SFML/Network/UdpSocket.hpp>

#include <vector>

/**
 * The server will handle packets and forward them to the application
 */
class ServerLayer : public Layer
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


    ServerLayer();

    virtual void OnUpdate() override final;
    virtual void OnNotify(Event& event) override final;

private:

    void Send(User& user, const PacketData& data);
    void SendAll(PacketData&& data);
    const User* GetUser(const Address& address) const;

    bool m_isHosting;
    sf::UdpSocket m_socket;
    Config m_config;
    
    std::vector<User> m_users;
};