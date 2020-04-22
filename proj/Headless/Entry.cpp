#include "common/Application.hpp"
#include "net/Server.hpp"

#include <iostream>

class Headless : public Layer
{
private:
    virtual void OnInit() override final
    {
        Server server;
        Server::Config conf;
        conf.address = sf::IpAddress::LocalHost;
        conf.port = 25565;

        server.Run(conf);
    }


    virtual void OnUpdate() override final
    {
    }
    virtual void OnNotify(Event& event) override final
    {
    }
};

std::unique_ptr<Layer> CreateApplication()
{
    return std::make_unique<Headless>();
}