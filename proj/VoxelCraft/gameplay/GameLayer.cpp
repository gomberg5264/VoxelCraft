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

    //m_model.m_players.push_back(Player());
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

    if (d.Dispatch<NetConnectResponseEvent>([&](NetConnectResponseEvent& e)
        {
            if (e.status == NetConnectResponseEvent::Status::Success)
                std::cout << "JOINED SERVER\n";
            else if (e.status == NetConnectResponseEvent::Status::Failed)
            {
                std::cout << "FAILED TO JOIN SERVER\n";
                ShouldPop();
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
                auto msg = ExtractPacket<GameMessagePacket>(e.packet);
                std::cout << msg.message << '\n';
            }
            break;
            case GamePacketType::Initial:
            {
                if (m_id == 0)
                {
                    auto data = ExtractPacket<GameInitialPacket>(e.packet);
                    m_id = data.id;
                    m_view.id = data.id;
                    m_model.m_players = data.players;
                    std::cout << "Received initial data from server\n";
                    std::cout << "Player count: " << m_model.m_players.size() << '\n';
                }
            }
            break;
            case GamePacketType::Join:
            {
                auto data = ExtractPacket<GameJoinPacket>(e.packet);
                m_model.m_players.push_back(data.player);
                std::cout << "Someone has joined: player count " << m_model.m_players.size() << '\n';

            }
            break;
            case GamePacketType::Move:
            {
                auto data = ExtractPacket<GameMovePacket>(e.packet);
                for(auto& player: m_model.m_players)
                    if (player.m_id == data.player.m_id)
                    {
                        player = data.player;
                    }
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
    
    for(auto& player : m_model.m_players)
        if (player.m_id == m_id)
        {
            // TODO: This should NOT modify the actual player pos, although client side prediction should happen
            // the server should stay authorative. 
            player.m_transform.m_pos = m_camera.get()->m_eye;
            Publish(NetClientPacketSendEvent(GameMovePacket(player).Build()));
        }

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

    if (d.Dispatch<NetJoinedEvent>([&](NetJoinedEvent& e)
        {
            // Send the connected player the current world state
            static unsigned uniqueID = 1;

            Player newPlayer;
            newPlayer.m_id = uniqueID++;

            std::cout << "Gave player " << e.user.name << " id " << newPlayer.m_id << '\n';

            auto initialPacket = GameInitialPacket();
            initialPacket.players = m_model.m_players;
            initialPacket.id = newPlayer.m_id;
            Publish(NetServerPacketSendEvent(initialPacket.Build()));

            // Add the new player
            m_model.m_players.push_back(newPlayer);
            Publish(NetServerPacketSendEvent(GameJoinPacket(newPlayer).Build()));
        })) return;

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
            //case GamePacketType::Initial:
            //{
            //    if (m_id == 0)
            //    {
            //        auto data = ExtractPacket<GameInitialPacket>(e.packet);
            //        m_id = data.id;
            //        m_model.m_players = data.players;
            //    }
            //}
            //break;
            //case GamePacketType::Join:
            //{
            //    auto data = ExtractPacket<GameJoinPacket>(e.packet);
            //    if (data.player.m_id != m_id)
            //    {
            //        m_model.m_players.push_back(data.player);
            //    }
            //}
            //break;
            case GamePacketType::Move:
            {
                auto data = ExtractPacket<GameMovePacket>(e.packet);
                // TODO: Verification

                Publish(NetServerPacketSendEvent(GameMovePacket(data.player).Build()));
            }
            break;
            }
        })) return;
}

void GameServerLayer::OnUpdate()
{
}