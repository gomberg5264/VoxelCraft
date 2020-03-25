#include "vcpch.hpp"

void Engine::Run()
{
    sf::Clock clock;
    clock.restart();

    OnInit();

    while(!m_shouldStop)
    {

        OnUpdate(clock.restart().asSeconds());
    }
}

void Engine::Stop()
{
    m_shouldStop = true;
}