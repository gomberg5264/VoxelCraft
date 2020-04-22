#include "vcpch.hpp"

#include "utils/Math.hpp"

#include "common/Application.hpp"
#include "common/Player.hpp"

#include "common/BlockData.hpp"
#include "common/Chunk.hpp"

#include "client/ChunkManager.hpp"
#include "client/Camera.hpp"

#include "client/gl/Texture.hpp"
#include "client/gl/Window.hpp"
#include "client/gl/ChunkRenderer.hpp"
#include "client/gl/PlayerRenderer.hpp"
#include "client/gl/SkyRenderer.hpp"

// TODO: This is a client engine instance. Move it to a dedicated file
class Game : public Layer
{
private:
    virtual void OnInit() override final
    {
        sf::Clock time;
        m_window.GetWindow().setMouseCursorVisible(false);

        // Setup camera
        // ---
        {
            m_camera = std::make_unique<FreelookCamera>(m_window.GetWindow());
            m_camera->m_eye = glm::vec3(0, 20, 10);
            m_camera->m_target = m_camera->m_eye + Transform::Forward();
            auto* cast = static_cast<FreelookCamera*>(m_camera.get());
            cast->m_speed = 5.f;
            cast->m_sensitivity = 0.2f;
        }

        // Create chunk manager
        m_chunkManager = std::make_unique<ChunkManager>(m_chunkRenderer);
        m_chunkManager->SetRadius(10.f * chunkDimension.x);

        m_chunkRenderer.SetSkyLightColor(glm::vec3(1));
        m_chunkRenderer.SetSkyLightDirection(glm::normalize(glm::vec3(-1, -3, -2)));
        m_chunkRenderer.SetSkyIntensity(0.6f);

        m_playerMesh = std::make_unique<PlayerMesh>(m_player.m_transform);

        std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
    }

    virtual void OnUpdate() override final
    {
        // Process events
        // TODO: We may want to forward these events
        sf::Event event;
        while (m_window.GetWindow().pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                m_window.GetWindow().close();
                Exit();
            }

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                m_window.GetWindow().close();
                Exit();
            }

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // UPDATE
        // ---------------------
        const float et = Core::time.Total();
        const float dt = Core::time.Elapsed();
        // TODO: temp day cycle
        constexpr float dayDur = 1.f / 8.0f;
        const float time = et * dayDur;
        m_camera->Update(dt);

        m_chunkManager->SetPos(m_camera->m_eye);
        m_chunkManager->Update();

        // TODO: Move this to a player class
        m_player.m_transform.m_pos = glm::vec3(0, 20, 0);
        m_player.m_transform.m_scale = glm::vec3(glm::sin(et), glm::cos(et), glm::sin(glm::cos(et))) * 10.f;
        m_player.m_transform.m_euler = m_player.m_transform.m_euler + glm::vec3(dt, dt * 3.f, dt * 1.5f);

        //std::printf("Cam x:%.2f y:%.2f z:%.2f\n",
        //    camPos.x,
        //    camPos.y,
        //    camPos.z);

        // RENDERING
        // ---------------------
        // Make draw requests to renderers
        m_chunkManager->Render();

        // Set up renderers state
        // Update sky renderer by passing current time
        m_skyRenderer.SetTime(std::fmod(time, 1.f));

        // Set up ligting for renderers
        m_chunkRenderer.SetSkyIntensity(m_skyRenderer.GetSkyAmbient());
        m_chunkRenderer.SetSkyLightDirection(m_skyRenderer.GetLightDir());
        m_chunkRenderer.SetSkyLightColor(m_skyRenderer.GetColor());
        m_chunkRenderer.SetDiffuseIntensity(m_skyRenderer.GetIntensity());

        // Set up camera matrices
        m_skyRenderer.SetCameraRotateProject(m_camera->GetProjection() * m_camera->GetRotation());
        m_chunkRenderer.SetVP(m_camera->GetProjection() * m_camera->GetView());

        // Render contents to the screen
        m_window.Clear();

        m_chunkRenderer.Display();
        m_skyRenderer.Display();

        // TODO: Make a player renderer/entity renderer
        m_playerMesh->Draw(*m_camera);

        m_window.Display();
    }

    Window m_window;
    std::unique_ptr<ChunkManager> m_chunkManager;
    ChunkRenderer m_chunkRenderer;
    SkyRenderer m_skyRenderer;

    Player m_player;
    std::unique_ptr<PlayerMesh> m_playerMesh;

    std::unique_ptr<Camera> m_camera;
};

std::unique_ptr<Layer> CreateApplication()
{
    return std::make_unique<Game>();
}