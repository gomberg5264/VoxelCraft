#pragma once

class Window
{
public:
    struct Config
    {
        float x{ 1280 };
        float y{ 720 };

        std::string title{ "VoxelCraft" };
    };

    Window(Config config = {});
    sf::Vector2u GetSize() const;

    // For event handling and stuff, renderer should actually
    // be renamed to graphics since it now is responsible for graphics
    // and it should forward window events to the engine so that they can be
    // handled correctly, but for now I just pass the window
    sf::Window& GetWindow();

    void Clear();
    void Display();

private:
    sf::Window m_window;
};