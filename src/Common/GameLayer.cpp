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

    m_shader = sh::Shader::Create("res/shaders/Face.glsl");
    
    m_cube = sh::VertexArray::Create();
    //m_cube->AddVertexBuffer(Face::CreateVertexBuffer(BlockFace::Front, 0, 0, -10.f, 0));
    m_cube->SetIndexBuffer(Cube::CreateIndexBuffer());
    auto vertices = Cube::CreateVertices(0, 0, -10.f, 0);
    auto buffer = Cube::CreateVertexBuffer();
    buffer->SetData(vertices.data(), vertices.size() * sizeof(vertices.front()));
    m_cube->AddVertexBuffer(buffer);

    m_texture = sh::Texture2D::Create("res/texture.png");
    m_texture->Bind();
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
    sh::Renderer::Submit(m_shader, m_cube, m_transform.GetWorldMatrix());
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