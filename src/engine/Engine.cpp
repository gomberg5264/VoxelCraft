
#include "vcpch.hpp"

Engine::Engine(Config config)
{
    // Create the main window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    
    m_window.create(sf::VideoMode(config.x, config.y), config.title.c_str(), sf::Style::Default, contextSettings);
    m_window.setActive();

    // Load OpenGL functions
    if (!gladLoadGL()) {
        throw "Glad couldn't be loaded!";
    }
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    // Configure OpenGL
    glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
    glClearColor(0.2f, 0.f, 0.2f, 1.f);
    glEnable(GL_DEPTH_TEST);
}

void Engine::Run()
{
    m_clock.restart();

    while(m_window.isOpen())
    {
        // Process events
        // We may want to forward these events
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                m_window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                m_window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        OnUpdate(m_clock.restart().asSeconds());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        OnRender();

        m_window.display();
    }
}

sf::Vector2u Engine::GetWindowSize() const
{
    return m_window.getSize();
}
