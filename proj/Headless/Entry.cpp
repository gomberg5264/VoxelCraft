#include "common/Application.hpp"

#include "gameplay/GameLayer.hpp"

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

void CreateApplication(Application::Layers& layers)
{
    layers.push_back(std::make_unique<GameServerLayer>());
}