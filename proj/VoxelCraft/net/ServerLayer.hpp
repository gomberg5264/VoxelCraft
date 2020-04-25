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

private:
    virtual void OnUpdate() override final;
    virtual void OnNotify(Event& event) override final;

    void Send(User& user, const Packet& packet);
    void SendAllBut(User& user, const Packet& packet);
    void SendAll(const Packet& packet);
    const User* GetUser(const Address& address) const;

    bool m_isHosting;
    sf::UdpSocket m_socket;
    Config m_config;
    
    std::vector<User> m_users;
};

inline std::string GenUserString(const std::string& name)
{
    return std::string("[") + name + "] ";
}