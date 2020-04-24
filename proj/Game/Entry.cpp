#include "common/Application.hpp"
#include "gameplay/GameLayer.hpp"


//// TODO: This is a client engine instance. Move it to a dedicated file
//class Game : public Engine
//{
//private:
//
//    virtual void OnUpdate(Time dt) override final
//    {
//        // Process events
//        // TODO: We may want to forward these events
//        sf::Event event;
//        while (m_window.GetWindow().pollEvent(event))
//        {
//            // Close window: exit
//            if (event.type == sf::Event::Closed)
//            {
//                m_window.GetWindow().close();
//                Stop();
//            }
//        
//            // Escape key: exit
//            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
//            {
//                m_window.GetWindow().close();
//                Stop();
//            }
//        
//            // Resize event: adjust the viewport
//            if (event.type == sf::Event::Resized)
//                glViewport(0, 0, event.size.width, event.size.height);
//        }
//
//        // UPDATE
//        // ---------------------
//        static float et = 0;
//        et += dt;
//        // TODO: temp day cycle
//
//        m_camera->Update(dt);
//
//        m_chunkManager->SetPos(m_camera->m_eye);
//        m_chunkManager->Update();
//
//        //std::printf("Cam x:%.2f y:%.2f z:%.2f\n",
//        //    camPos.x,
//        //    camPos.y,
//        //    camPos.z);
//
//        // RENDERING
//        // ---------------------
//        // Make draw requests to renderers
//        m_chunkManager->Render();
//
//        // Set up renderers state
//        // Update sky renderer by passing current time
//        m_skyRenderer.SetTime(std::fmod(time, 1.f));
//        
//        // Set up ligting for renderers
//        m_chunkRenderer.SetSkyIntensity(m_skyRenderer.GetSkyAmbient());
//        m_chunkRenderer.SetSkyLightDirection(m_skyRenderer.GetLightDir());
//        m_chunkRenderer.SetSkyLightColor(m_skyRenderer.GetColor());
//        m_chunkRenderer.SetDiffuseIntensity(m_skyRenderer.GetIntensity());
//
//        // Set up camera matrices
//        m_skyRenderer.SetCameraRotateProject(m_camera->GetProjection() * m_camera->GetRotation());
//        m_chunkRenderer.SetVP(m_camera->GetProjection() * m_camera->GetView());
//
//        // Render contents to the screen
//        m_window.Clear();
//
//        m_chunkRenderer.Display();
//        m_skyRenderer.Display();
//
//        // TODO: Make a player renderer/entity renderer
//        m_playerMesh->Draw(*m_camera);
//
//        m_window.Display();
//    }
//
//    std::unique_ptr<ClientChunkManager> m_chunkManager;
//    ChunkRenderer m_chunkRenderer;
//    SkyRenderer m_skyRenderer;
//
//    Player m_player;
//    std::unique_ptr<PlayerMesh> m_playerMesh;
//
//    std::unique_ptr<Camera> m_camera;
//};

void CreateApplication(Application::Layers& layers)
{
    layers.push_back(std::make_unique<GameClientLayer>());
}