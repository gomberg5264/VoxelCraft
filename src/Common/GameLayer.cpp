#include "Common/GameLayer.h"
#include "Client/Primitive.h"

GameLayer::GameLayer()
    : sh::Layer("Example Layer")
    , m_camera(glm::radians(45.f), 16.f / 9.f, 0.1f, 1000.f)
{
}

static bool look = false;

void GameLayer::OnAttach()
{
    // This has to be called when attaching a new layer. See the function for more details
    ImGui::SetCurrentContext(sh::ImGuiLayer::GetContext());

    m_texture = sh::Texture2D::Create("res/texture.png");
    m_texture->Bind();

    m_players.reserve(3);

    m_players.emplace_back();
    m_input.player = &m_players.back();

    m_players.emplace_back();
    m_players.back().transform.Move(sh::Transform::GetWorldRight() * 4.f);
    m_players.emplace_back();
    m_players.back().transform.Move(sh::Transform::GetWorldRight() * 8.f);
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
        if (&p == m_input.player)
            continue;

        if (look)
             p.transform.LookAt(m_camera.transform.GetPosition());
        else
            p.transform.SetRotation(glm::vec3(0));
    }
    m_camera.transform = m_input.player->transform;
    
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

    if (!ImGui::Begin("Camera"))
    {
        ImGui::End();
        return;
    }

    auto pos = m_camera.transform.GetPosition();
    ImGui::DragFloat3("pos", &pos.x);

    for (int i = 0; i < m_players.size(); i++)
    {
        std::string label = "player ";
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