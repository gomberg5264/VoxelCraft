#include "Common/GameLayer.h"
#include "Client/Primitive.h"

GameLayer::GameLayer()
    : sh::Layer("Example Layer")
    , m_camera(glm::radians(45.f), 16.f / 9.f)
{
}

void GameLayer::OnAttach()
{
    // This has to be called when attaching a new layer. See the function for more details
    ImGui::SetCurrentContext(sh::ImGuiLayer::GetContext());

    m_texture = sh::Texture2D::Create("res/texture.png");
    m_texture->Bind();


    m_players.emplace_back();
    m_players.emplace_back();
    m_players.back().transform.Move(sh::Transform::GetWorldRight() * 4.f);
}

void GameLayer::OnEvent(sh::Event& event)
{
    m_camera.OnEvent(event);

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
    m_camera.OnUpdate(ts);

    sh::Renderer::BeginScene(m_camera.GetCamera());
    for(const auto& p : m_players)
        p.Draw();
    sh::Renderer::EndScene();
}

void GameLayer::OnGuiRender()
{
    if (!ImGui::Begin("Camera"))
    {
        ImGui::End();
        return;
    }

    auto pos = m_camera.GetCamera().GetViewMatrix()[3];
    ImGui::DragFloat3("pos", &pos.x);

    ImGui::End();
}