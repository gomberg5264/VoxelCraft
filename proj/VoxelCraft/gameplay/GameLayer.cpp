#include "vcpch.hpp"
#include "gameplay/GameLayer.hpp"

void GameClientLayer::OnInit()
{
    sf::Clock time;
    m_window.SetCursorVisible(false);

    // Setup camera
    // ---
    {
        m_camera = std::make_unique<FreelookCamera>(m_window);
        m_camera->m_eye = glm::vec3(0, 20, -10);
        m_camera->m_target = m_camera->m_eye + Transform::Forward();
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
    }

    m_model.m_players.push_back(Player());
    //m_strategy.SetRadius(10);

    m_model.m_chunks.m_addCB = [&](Chunk& chunk)
    {
        m_view.AddChunk(chunk);
    };
    m_model.m_chunks.m_removeCB = [&](Chunk& chunk)
    {
        m_view.RemoveChunk(chunk);
    };

    std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
}

void GameClientLayer::OnNotify(Event& event)

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

void GameClientLayer::OnUpdate()
{
    //m_controller.Update(m_model);
    m_window.PollEvents(GetApplication());

    // Temp, this should be generated in view based on player orientation
    m_camera->Update(Core::time.Elapsed());

    m_model.Update();
    m_strat.SetPos(m_camera.get()->m_eye);
    m_strat.Update(m_model.m_chunks);
    //m_model.m_chunks.AddChunk(m_camera->m_eye);

    m_window.Clear();
    m_view.Draw(m_model, *m_camera);
    m_window.Display();
}