#include "Common/GameLayer.h"
#include "Client/Primitive.h"


std::vector<Player> GameLayer::m_players;
static bool look = false;

GameLayer::GameLayer()
    : sh::Layer("Example Layer")
    , m_camera(glm::radians(45.f), 16.f / 9.f, 0.1f, 1000.f)
{
}

void GameLayer::OnAttach()
{
    // This has to be called when attaching a new layer. See the function for more details
    ImGui::SetCurrentContext(sh::ImGuiLayer::GetContext());

    m_texture = sh::Texture2D::Create("res/texture.png");
    m_texture->Bind();

    m_players.reserve(3);

    m_players.emplace_back(0);
    m_input.playerID = m_players.back().id;

    m_players.emplace_back(1);
    m_players.back().transform.Move(sh::Transform::GetWorldRight() * 4.f);
    m_players.emplace_back(2);
    m_players.back().transform.Move(sh::Transform::GetWorldRight() * 8.f);

    m_input.callback = [](Command& command)
    {
        command.Execute();
    };
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnEvent(sh::Event& event)
{
    //m_camera.OnEvent(event);
    m_input.OnEvent(event);

    sh::EventDispatcher d(event);
    d.Dispatch<sh::KeyPressedEvent>([&](sh::KeyPressedEvent& e)
        {
            if (e.GetKeyCode() == sh::KeyCode::Escape)
                sh::Application::Get().Exit();
            return false;
        });
}


void GameLayer::OnUpdate(sh::Timestep ts)
{
    //m_camera.OnUpdate(ts);
    m_input.OnUpdate(ts);

    if (m_client.IsConnected())
    {
        ENetEvent event;
        while(m_client.Poll(event) > 0)
        { 
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                SH_TRACE("Connected from {0}:{1}", event.peer->address.host, event.peer->address.port);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                SH_TRACE("Disconnected from {0}:{1}", event.peer->address.host, event.peer->address.port);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                SH_TRACE("Received packet from server {0}:{1}", event.peer->address.host, event.peer->address.port);
                enet_packet_destroy(event.packet);
                break;
            }
        }
    }


    for (auto& p : m_players)
    {
        if (p.id == m_input.playerID)
            continue;

        if (look)
             p.transform.LookAt(m_camera.transform.GetPosition());
        else
            p.transform.SetRotation(glm::vec3(0));
    }
    m_camera.transform = 
        std::find_if(std::begin(m_players), std::end(m_players), [&](const Player& p)
        { return p.id == m_input.playerID;} )->transform;
    
    sh::Renderer::BeginScene(m_camera);
    for (auto& p : m_players)
    {
        p.Draw();
    }
    sh::Renderer::EndScene();
}

void GameLayer::OnGuiRender()
{
    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);

    // Net widget
    {
        if (!ImGui::Begin("Network"))
            ImGui::End();

        // Server
        {
            static bool server = false;
            if (ImGui::Checkbox("server", &server))
            {
                if (server)
                {
                    server = m_server.Host(25565);

                    m_serverThread.swap(std::thread([&]()
                        {
                            while (m_server.IsHosting())
                            {
                                ENetEvent event;
                                while (m_server.Poll(event) > 0)
                                {
                                    switch (event.type)
                                    {
                                    case ENET_EVENT_TYPE_CONNECT:
                                        SH_TRACE("Client connected from {0}:{1}", event.peer->address.host, event.peer->address.port);
                                        break;
                                    case ENET_EVENT_TYPE_DISCONNECT:
                                        SH_TRACE("Client disconnected from {0}:{1}", event.peer->address.host, event.peer->address.port);
                                        break;
                                    case ENET_EVENT_TYPE_RECEIVE:
                                        SH_TRACE("Received packet from client {0}:{1}", event.peer->address.host, event.peer->address.port);
                                        enet_packet_destroy(event.packet);
                                        break;
                                    }
                                }
                            }
                        }));
                }
                else
                {
                    m_server.Close();
                    m_serverThread.join();
                }

            }
        }

        // Client
        {
            static bool client = false;
            if (ImGui::Checkbox("client", &client))
            {
                if (client)
                {
                    client = m_client.Connect("localhost", 25565);
                }
                else
                {
                    m_client.Disconnect();
                }
            }
        }

        ImGui::End();
    }

    // Camera widget
    /*
    {
        if (!ImGui::Begin("Camera"))
        {
            ImGui::End();
            return;
        }

        auto pos = m_camera.transform.GetPosition();
        ImGui::DragFloat3("pos", &pos.x);

        for (int i = 0; i < m_players.size(); i++)
        {
            std::string label = "playerID ";
            label += std::to_string(i);

            if (ImGui::TreeNode(label.c_str()))
            {
                auto p = m_players[i].transform.GetPosition();
                ImGui::DragFloat3("actual Pos", &p.x);

                auto pos = m_players[i].transform.GetWorldMatrix()[3];
                ImGui::DragFloat3("pos", &pos.x);
                
                auto rot = m_players[i].transform.GetEulerRotation();
                ImGui::DragFloat3("rot", &rot.x);
                
                ImGui::TreePop();
            }
        }

        ImGui::Checkbox("Look at user", &look);

        ImGui::End();
    }
    */
}