#include "GameLayer.h"

GameLayer::GameLayer()
    : sh::Layer("Example Layer")
    , fov(glm::radians(45.f))
    , m_camera(fov, 16.f / 9.f)
{}

void GameLayer::OnAttach()
{
    // This has to be called when attaching a new layer. See the function for more details
    ImGui::SetCurrentContext(sh::ImGuiLayer::GetContext());

    // Create a cube
    {
        //https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/Creating_3D_objects_using_WebGL
        constexpr float positions[]{
            // Front face
                -1.0, -1.0, 1.0, 0.f, 0.f,
                 1.0, -1.0, 1.0, 1.f, 0.f,
                 1.0,  1.0, 1.0, 1.f, 1.f,
                -1.0,  1.0, 1.0, 0.f, 1.f,

                // Back face
                -1.0, -1.0, -1.0, 1.f, 1.f,
                -1.0,  1.0, -1.0, 1.f, 0.f,
                 1.0,  1.0, -1.0, 0.f, 0.f,
                 1.0, -1.0, -1.0, 0.f, 1.f,

                 // Top face
                 -1.0, 1.0, -1.0, 0.f, 1.f,
                 -1.0, 1.0,  1.0, 0.f, 0.f,
                  1.0, 1.0,  1.0, 1.f, 0.f,
                  1.0, 1.0, -1.0, 1.f, 1.f,

                  // Bottom face
                  -1.0, -1.0, -1.0, 0.f, 1.f,
                   1.0, -1.0, -1.0, 1.f, 1.f,
                   1.0, -1.0,  1.0, 1.f, 0.f,
                  -1.0, -1.0,  1.0, 0.f, 0.f,

                  // Right face
                  1.0, -1.0, -1.0, 1.f, 0.f,
                  1.0,  1.0, -1.0, 1.f, 1.f,
                  1.0,  1.0,  1.0, 0.f, 1.f,
                  1.0, -1.0,  1.0, 0.f, 0.f,

                  // Left face
                  -1.0, -1.0, -1.0, 0.f, 0.f,
                  -1.0, -1.0,  1.0, 1.f, 0.f,
                  -1.0,  1.0,  1.0, 1.f, 1.f,
                  -1.0,  1.0, -1.0, 0.f, 1.f,
        };
        constexpr uint32_t indices[]{
            0, 1, 2, 0, 2, 3,    // front
            4, 5, 6, 4, 6, 7,    // back
            8, 9, 10, 8, 10, 11,   // top
            12, 13, 14, 12, 14, 15,   // bottom
            16, 17, 18, 16, 18, 19,   // right
            20, 21, 22, 20, 22, 23,   // left
        };

        m_shader = sh::Shader::Create("res/shaders/Texture3D.glsl");

        auto vBuffer = sh::VertexBuffer::Create(positions, sizeof(positions));
        vBuffer->AddElement(sh::BufferElement(sh::ShaderDataType::Float3, "aPos"));
        vBuffer->AddElement(sh::BufferElement(sh::ShaderDataType::Float2, "aTex"));

        m_texture = sh::Texture2D::Create("res/image.png");
        m_texture->Bind();

        auto iBuffer = sh::IndexBuffer::Create(indices, sizeof(indices) / sizeof(indices[0]));

        m_cube = sh::VertexArray::Create();
        m_cube->AddVertexBuffer(vBuffer);
        m_cube->SetIndexBuffer(iBuffer);
    }
    m_transform.SetPosition(glm::vec3(0, 0, 10));

    m_camera.GetCamera().transform.LookTowards(sh::Transform::GetWorldForward());
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
    static float et = 0.f;
    et += ts;

    // Update camera
    const float rotX = glm::sin(glm::radians(et * 180.f));
    const float rotY = glm::cos(glm::radians(et * 90.f));
    const float rotZ = glm::sin(glm::cos(glm::radians(et * 180.f)));
    const float xOffset = glm::cos(glm::radians(et * 180.f));
    const float yOffset = glm::sin(glm::radians(et * 180.f));
    const glm::vec3 offset = glm::vec3(xOffset, yOffset, 0.f) * 3.f;
    auto transform =
        glm::translate(glm::mat4(1), offset + sh::Transform::GetWorldForward() * 10.f) *
        glm::rotate(glm::mat4(1), rotX, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1), rotY, glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1), rotZ, glm::vec3(0, 0, 1))
        ;

    m_transform.SetPosition(transform[3]);

    if (m_lookAtCube)
        m_camera.GetCamera().transform.LookAt(m_transform.GetPosition());

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

    ImGui::SliderAngle("fov", &fov, 0, 180);
    m_camera.GetCamera().SetFOV(fov);

    {
        glm::vec3 pos(m_camera.GetCamera().transform.GetPosition());
        ImGui::DragFloat3("position", &pos.x);
        glm::vec3 f(m_camera.GetCamera().transform.GetForward());
        ImGui::DragFloat3("forward", &f.x);
        glm::vec3 r(m_camera.GetCamera().transform.GetRight());
        ImGui::DragFloat3("right", &r.x);
        glm::vec3 u(m_camera.GetCamera().transform.GetUp());
        ImGui::DragFloat3("up", &u.x);
    }

    {
        glm::vec3 t(m_camera.GetCamera().transform.GetEulerRotation());
        ImGui::DragFloat3("rotation", &sh::Degrees(t).x);
        m_camera.GetCamera().transform.SetRotation(sh::Radians(t));
    }

    {
        glm::vec3 t(m_transform.GetEulerRotation());
        ImGui::DragFloat3("cube rotation", &sh::Degrees(t).x);
        m_transform.SetRotation(sh::Radians(t));
    }


    if (ImGui::Button("reset")) m_transform.SetRotation(glm::vec3(0));
    if (ImGui::Checkbox("look at cube", &m_lookAtCube)); // Yea we don't do anything here cuz we check in update

    ImGui::End();
}