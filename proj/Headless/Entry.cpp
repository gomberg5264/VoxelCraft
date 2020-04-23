#include "common/Application.hpp"

#include "net/ServerLayer.hpp"
#include "net/ClientLayer.hpp"

#include <iostream>

class Game : public Layer
{
private:
    virtual void OnInit() override final
    {
        Address server;
        server.ip = sf::IpAddress::LocalHost;
        server.port = 25565;
        Publish(NetConnectEvent(server,"VoxelCraft"));
    }
};

class Headless : public Layer
{
private:
    virtual void OnInit() override final
    {
        ServerLayer::Config conf;
        conf.address.ip = sf::IpAddress::LocalHost;
        conf.address.port = 25565;

        Publish(NetHostEvent(conf));
    }
};

void CreateApplication(Application::Layers& layers)
{
    std::cout << "0 server 1 client\n";

    int i;
    std::cin >> i;
    if (i == 0)
    {
        layers.push_back(std::make_unique<ServerLayer>());
        layers.push_back(std::make_unique<Headless>());
    }
    else
    {
        layers.push_back(std::make_unique<ClientLayer>());
        layers.push_back(std::make_unique<Game>());
    }
}