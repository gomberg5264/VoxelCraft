#include "common/Application.hpp"
#include "net/Client.hpp"
#include "net/Server.hpp"

#include <iostream>

class Game : public Layer
{
private:
    virtual void OnInit() override final
    {
        Client client;
        Address server;
        server.ip = sf::IpAddress::LocalHost;
        server.port = 25565;

        client.Connect(server, "VoxelCraft");
    }


    virtual void OnUpdate() override final
    {
    }
    virtual void OnNotify(Event& event) override final
    {
    }
};

class Headless : public Layer
{
private:
    virtual void OnInit() override final
    {
        Server::Config conf;
        conf.address.ip = sf::IpAddress::LocalHost;
        conf.address.port = 25565;

        m_server.Host(conf);
    }


    virtual void OnUpdate() override final
    {
        m_server.PollEvents(GetApplication());
    }
    virtual void OnNotify(Event& event) override final
    {
    }
    
    Server m_server;

};

std::unique_ptr<Layer> CreateApplication()
{
    std::cout << "0 server 1 client\n";

    int i;
    std::cin >> i;
    if (i == 0) return std::make_unique<Headless>();
    return std::make_unique<Game>();
}