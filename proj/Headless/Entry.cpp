#include "common/Application.hpp"

#include "net/ServerLayer.hpp"
#include "net/ClientLayer.hpp"

#include "utils/Helper.hpp"

#include <iostream>
#include <thread>

//class Game : public Layer
//{
//private:
//    virtual void OnInit() override final
//    {
//        std::cout << "Enter your name: ";
//        std::string name = GetLine();
//
//        Address server;
//        std::cout << "Enter server IP address (empty assumes localhost): ";
//        std::string ip = GetLine();
//        server.ip = ip.empty() ? sf::IpAddress::LocalHost : ip;
//        server.port = 25565;
//        Publish(NetConnectEvent(server,name.c_str()));
//    }
//
//    virtual void OnNotify(Event& e) override final
//    {
//        EventDispatcher d(e);
//    }
//
//    virtual void OnDeinit() override final
//    {
//        std::cout << "Exiting application\n";
//        if(m_console.joinable()) m_console.join();
//    }
//
//    std::thread m_console;
//};
//
//class Headless : public Layer
//{
//private:
//    virtual void OnInit() override final
//    {
//        ServerLayer::Config conf;
//        conf.address.ip = sf::IpAddress::LocalHost;
//        conf.address.port = 25565;
//
//        Publish(NetHostEvent(conf));
//    }
//
//    virtual void OnNotify(Event& e) override final
//    {
//        EventDispatcher d(e);
//        d.Dispatch<NetHostResponseEvent>([&](NetHostResponseEvent& e)
//            {
//                if (e.status == NetHostResponseEvent::Status::Success)
//                {
//                    m_t = std::thread([&]()
//                        {
//                            bool quit = false;
//                            while (!quit)
//                            {
//                                std::string msg = GetLine();
//
//                                if (msg == "/q")
//                                    quit = true;
//                                Publish(NetMessageEvent(msg));
//                            }
//                            Publish(NetShutdownEvent());
//                            Exit();
//                        });
//                }
//                else
//                {
//                    std::cout << "Could not start up server\n";
//                    Exit();
//                }
//            });
//    }
//
//    virtual void OnDeinit() override final
//    {
//        if (m_t.joinable()) m_t.join();
//    }
//
//    std::thread m_t;
//};
//
//void CreateApplication(Application::Layers& layers)
//{
//    std::cout << "0 server 1 client\n";
//
//    int i;
//    std::cin >> i;
//    std::cin.ignore();
//    if (i == 0)
//    {
//        layers.push_back(std::make_unique<ServerLayer>());
//        layers.push_back(std::make_unique<Headless>());
//    }
//    else
//    {
//        layers.push_back(std::make_unique<ClientLayer>());
//        layers.push_back(std::make_unique<Game>());
//    }
//}

void CreateApplication(Application::Layers& layers)
{}