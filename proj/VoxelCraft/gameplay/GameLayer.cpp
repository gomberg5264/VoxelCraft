#include "vcpch.hpp"
#include "gameplay/GameLayer.hpp"
#include "gameplay/GamePacket.hpp"

#include "net/ClientLayer.hpp"
#include "net/ServerLayer.hpp"

#include "utils/Helper.hpp"

void GameClientLayer::InitGame()
{
    m_window.SetCursorVisible(false);

    // Setup camera
    // ---
    {
        m_camera = std::make_unique<FreelookCamera>(m_window);
        m_camera->m_eye = glm::vec3(0, 20, -10);
        m_camera->m_target = m_camera->m_eye + Transform::Forward();
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
    }

    m_model.m_players.push_back(Player());
    //m_strategy.SetRadius(10);

    m_model.m_chunks.m_addCB = [&](Chunk& chunk)
    {
        m_view.AddChunk(chunk);
    };
    m_model.m_chunks.m_removeCB = [&](Chunk& chunk)
    {
        m_view.RemoveChunk(chunk);
    };
}

void GameClientLayer::OnInit()
{
    //std::cout << "1 host 2 join\n";

    //int i;
    //std::cin >> i;
    //std::cin.ignore();
    //if (i == 1)
    //{
    //    GetApplication().AddLayer(std::make_unique<ServerLayer>());
    //    ServerLayer::Config conf;
    //    conf.address.ip = sf::IpAddress::LocalHost;
    //    conf.address.port = 25565;

    //    Publish(NetHostEvent(conf));
    //}

    GetApplication().AddLayer(std::make_unique<ClientLayer>());
    std::cout << "Enter your name: ";
    std::string name = GetLine();

    Address server;
    std::cout << "Enter server IP address (empty assumes localhost): ";
    std::string ip = GetLine();
    server.ip = ip.empty() ? sf::IpAddress::LocalHost : ip;
    server.port = 25565;
    Publish(NetConnectEvent(server,name.c_str()));

    sf::Clock time;
    InitGame();
    std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());

    // TEMP: Create a tread so that we can send messages
    m_console = std::thread([&]()
    {
        bool quit = false;
        while (!quit)
        {
            std::string msg = GetLine();
            // The network protocol will handle this event
            Publish(NetClientPacketSendEvent(GameMessagePacket(msg.c_str()).Build()));
        }
        Publish(NetDisconnectEvent());
        Exit();
    });
}

void GameClientLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    if (d.Dispatch<WindowCloseEvent>([&](Event& e) { Exit(); })) return;
    if (d.Dispatch<KeyPressEvent>([&](KeyPressEvent& e)
        {
            if (e.GetKeyCode() == sf::Keyboard::Escape)
            {
                m_window.Close();
                Exit();
            }
        })) return;

    if (d.Dispatch<NetClientPacketReceiveEvent>([&](NetClientPacketReceiveEvent& e)
        {
            GamePacketType type;
            e.packet >> type;
            
            switch (type)
            {
            case GamePacketType::Message:
            {
                auto msg= ExtractPacket<GameMessagePacket>(e.packet);
                std::cout << msg.message << '\n';
            }
            break;
            }
        })) return;
}

void GameClientLayer::OnUpdate()
{
    m_window.PollEvents(GetApplication());

    // Temp, this should be generated in view based on player orientation
    m_camera->Update(Core::time.Elapsed());

    m_model.Update();
    m_strat.SetPos(m_camera.get()->m_eye);
    m_strat.Update(m_model.m_chunks);
    //m_model.m_chunks.AddChunk(m_camera->m_eye);

    m_window.Clear();
    m_view.Draw(m_model, *m_camera);
    m_window.Display();
}

void GameServerLayer::OnInit()
{
    GetApplication().AddLayer(std::make_unique<ServerLayer>());
    ServerLayer::Config conf;
    conf.address.ip = sf::IpAddress::LocalHost;
    conf.address.port = 25565;

    Publish(NetHostEvent(conf));
}

void GameServerLayer::OnNotify(Event& event)
{
    EventDispatcher d(event);

    if (d.Dispatch<NetServerPacketReceiveEvent>([&](NetServerPacketReceiveEvent& e)
        {
            GamePacketType type;
            e.packet >> type;

            switch (type)
            {
            case GamePacketType::Message:
            {
                std::string msg;
                msg = GenUserString(e.user.name) + ExtractPacket<GameMessagePacket>(e.packet).message;
                std::cout << msg << '\n';
                
                Publish(NetServerPacketSendEvent(GameMessagePacket(msg.c_str()).Build()));
            }
            break;
            }
        })) return;
}

void GameServerLayer::OnUpdate()
{
}