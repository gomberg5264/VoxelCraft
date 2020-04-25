#pragma once
#include "common/Application.hpp"
#include "common/event/WindowEvent.hpp"
#include "common/event/InputEvent.hpp"

#include "gameplay/GameModel.hpp"
#include "gameplay/GameView.hpp"

#include "client/gl/Window.hpp"
#include "client/Camera.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <memory>


/**
 * Handles the data that goes to the GameModel
 */
class GameClientLayer : public Layer
{
private:
    virtual void OnInit() override final
    {
        sf::Clock time;
        //m_window.SetCursorVisible(false);

        // Setup camera
        // ---
        {
            m_camera = std::make_unique<FreelookCamera>(m_window);
            m_camera->m_eye = glm::vec3(0,20,-10);
            m_camera->m_target = m_camera->m_eye + Transform::Forward();
            auto* cast = static_cast<FreelookCamera*>(m_camera.get());
            cast->m_speed = 5.f;
            cast->m_sensitivity = 0.2f;
        }

        // Create chunk manager
        //m_chunkManager = std::make_unique<ChunkStrategy>(m_chunkRenderer);
        //m_chunkManager->SetRadius(10.f * chunkDimension.x);
        //
        //m_chunkRenderer.SetSkyLightColor(glm::vec3(1));
        //m_chunkRenderer.SetSkyLightDirection(glm::normalize(glm::vec3(-1, -3, -2)));
        //m_chunkRenderer.SetSkyIntensity(0.6f);

        //m_playerMesh = std::make_unique<PlayerMesh>(m_player.m_transform);

        m_model.m_players.push_back(Player());
        m_model.m_players.push_back(Player());

        std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
    }

    virtual void OnNotify(Event& event) override final
    {
        EventDispatcher d(event);

        if (d.Dispatch<WindowCloseEvent>([&](Event& e) { Exit(); })) return;
        //if (d.Dispatch<WindowResizeEvent>([&](Event& d) { Exit(); })) return;

        if (d.Dispatch<KeyPressEvent>([&](KeyPressEvent& e)
            {
                if (e.GetKeyCode() == sf::Keyboard::Escape)
                {
                    m_window.Close();
                    Exit();
                }


            })) return;
    }

    virtual void OnUpdate() override final
    {
        //m_controller.Update(m_model);
        m_window.PollEvents(GetApplication());
        
        // Temp, this should be generated in view based on player orientation
        m_camera->Update(Core::time.Elapsed());

        m_model.Update();
        m_view.AddChunk(m_model.m_chunks.AddChunk(m_camera->m_eye));

        m_window.Clear();
        m_view.Draw(m_model,*m_camera);
        m_window.Display();
    }

    Window m_window;
    std::unique_ptr<FreelookCamera> m_camera;

    GameModel m_model;
    GameView m_view;
};


class GameServerLayer : public Layer
{
public:

};