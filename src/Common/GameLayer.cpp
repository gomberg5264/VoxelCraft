#include "Common/GameLayer.h"
#include "Client/Primitive.h"


std::vector<Player> GameLayer::m_players;
static bool look = false;

static bool server = false;
static bool client = false;


GameLayer::GameLayer()
    : sh::Layer("Example Layer")
    , m_camera(glm::radians(45.f), 16.f / 9.f, 0.1f, 1000.f)
{
}

void GameLayer::OnAttach()
{
    enet_initialize();
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
    if (client) enet_host_destroy(m_client);
    if (server) enet_host_destroy(m_server);
    if (m_peer) enet_peer_reset(m_peer);

    enet_deinitialize();
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

        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = 25565;

        //enet_address_get_host(const ENetAddress * address, char* name, size_t nameLength)

        static char buf[64]; 
        enet_address_get_host_ip(&address, buf, 64u);
        ImGui::InputText("server", buf, 64);// , ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        if (ImGui::Checkbox("server", &server))
        {
            if (server)
            {
                m_server = enet_host_create(&address, 10, 1, 0, 0);
            }
            else
            {
                enet_host_destroy(m_server);
            }

        }

        static char buf2[64] = ""; 
        ImGui::InputText("client", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        if (ImGui::Checkbox("client", &client))
        {
            if (client)
            {
                ENetAddress address;
                address.host = ENET_HOST_ANY;
                address.port = ENET_PORT_ANY;

                m_client = enet_host_create(nullptr, 1, 1, 0, 0);
                m_peer = enet_host_connect(m_client, &address, 1, 0);

                if (!m_peer)
                {
                    SH_ERROR("No available peers for initiating an ENet connection");
                    client = false;
                    enet_host_destroy(m_client);
                }
                else
                {
                    ENetEvent event;
                    /* Wait up to 5 seconds for the connection attempt to succeed. */
                    if (enet_host_service(m_client, &event, 5000) > 0 &&
                        event.type == ENET_EVENT_TYPE_CONNECT)
                    {
                        SH_TRACE("Connection succeeded.");
                        //puts("Connection to some.server.net:1234 succeeded.");

                    }
                }
            }
            else
            {
                enet_host_destroy(m_client);
                enet_peer_reset(m_peer);
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