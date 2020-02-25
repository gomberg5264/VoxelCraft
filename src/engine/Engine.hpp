#pragma once

class Engine
{
public:
    using Time = float;

    struct Config
    {
        float x{ 1280 };
        float y{ 720 };
    
        std::string title{ "VoxelCraft" };
    };
    
    Engine(Config config = {});

    void Run();

protected:
    sf::Vector2u GetWindowSize() const;

private:
    virtual void OnUpdate(Time time) {}
    virtual void OnRender() {}

    sf::Clock m_clock;
    sf::Window m_window;
};