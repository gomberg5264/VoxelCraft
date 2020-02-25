#include "vcpch.hpp"

Engine::Engine(Config config)
    : m_renderer(config.graphics)
{
}

void Engine::Run()
{
    m_clock.restart();

    while(m_renderer.GetWindow().isOpen())
    {
        // Process events
        // We may want to forward these events
        sf::Event event;
        while (m_renderer.GetWindow().pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                m_renderer.GetWindow().close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                m_renderer.GetWindow().close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        OnUpdate(m_clock.restart().asSeconds());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        OnRender(m_renderer);

        m_renderer.Display();
    }
}

sf::Vector2u Engine::GetWindowSize() const
{
    return m_renderer.GetSize();
}
