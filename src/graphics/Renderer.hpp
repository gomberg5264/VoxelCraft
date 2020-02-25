#pragma once

class Renderer
{
public:
    struct Config
    {
        float x{ 1280 };
        float y{ 720 };

        std::string title{ "VoxelCraft" };
    };

    enum class Primitive
    {
        Face,
        Cube
    };

    Renderer(Config config);

    void SetMVP(const glm::mat4& mvp);

    /**
     * Make sure that the renderable is still alive
     */
    void Render(const Renderable &renderable);

    void Display();

    sf::Vector2u GetSize() const;

    // For event handling and stuff, renderer should actually
    // be renamed to graphics since it now is responsible for graphics
    // and it should forward window events to the engine so that they can be
    // handled correctly, but for now I just pass the window
    sf::Window& GetWindow();

private:
    std::vector<const Renderable*> m_renderables;

    sf::Window m_window;
    Shader m_shad;
};