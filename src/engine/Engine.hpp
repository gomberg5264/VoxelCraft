#pragma once

class Engine
{
public:
    using Time = float;

    struct Config
    {
        Renderer::Config graphics;
    };
    
    Engine(Config config = {});

    void Run();

protected:
    sf::Vector2u GetWindowSize() const;

private:
    virtual void OnUpdate(Time time) {}
    virtual void OnRender(Renderer& renderer) {}

    Renderer m_renderer;
    sf::Clock m_clock;
};