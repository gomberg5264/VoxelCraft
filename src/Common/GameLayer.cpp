#include "Common/GameLayer.h"
#include "Client/Primitive.h"

#include "Net/Packet.h"

#include "Player.h"

std::vector<Player> GameLayer::m_players;
static bool look = false;

static std::vector<std::unique_ptr<Command>> m_commands;

// TODO: Find a better way to do the server update loop
static bool serverLoop = false;

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
    m_players.emplace_back(1);
    m_players.back().transform.Move(-sh::Transform::GetWorldForward() * 4.f);
    m_players.emplace_back(2);
    m_players.back().transform.Move(-sh::Transform::GetWorldForward() * 8.f);
}

void GameLayer::OnDetach()
{
    // TODO: Gracefully shut down server instead of forcing
    serverLoop = false;
    if(m_serverThread.joinable()) m_serverThread.join();
}

void GameLayer::OnEvent(sh::Event& event)
{
    //m_camera.OnEvent(event);
    if(m_input) m_input->OnEvent(event);

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
    if(m_input) m_input->OnUpdate(ts);

    if (m_client.IsConnected())
    {
        // Send all the packets
        // Gotta figure out how to serialize dem commands
        for (const auto& command : m_commands)
        {
            // Get command
            // Serialize command
            // Send command to server
            // Verify command
            // Send authorative command back

            sf::Packet data;

            // TODO: Make a nice interface such as this one
            // I think that I can use Cereal
            //data << command;

            

            auto c = dynamic_cast<MoveCommand*>(command.get());
            data << COMMAND_TYPE_MOVE << c->m_player.id << c->m_pos << c->m_oldPos;

            auto* pck = enet_packet_create(data.getData(), data.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
            m_client.SendPacket(pck);
        }
        m_commands.clear();

        // Handle events
        ENetEvent event;
        while(m_client.Poll(event) > 0)
        { 
        }
    }

    if (m_input)
    {

    for (auto& p : m_players)
    {
        if (p.id == m_input->playerID)
            continue;

        if (look)
             p.transform.LookAt(m_camera.transform.GetPosition());
        else
            p.transform.SetRotation(glm::vec3(0));
    }
    m_camera.transform = 
        std::find_if(std::begin(m_players), std::end(m_players), [&](const Player& p)
        { return p.id == m_input->playerID;} )->transform;
    }

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