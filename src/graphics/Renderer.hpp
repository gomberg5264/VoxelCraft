#pragma once

class Renderer
{
public:
    struct Config
    {
        float x{ 1280 };
        float y{ 720 };

        unsigned atlasX{ 2 };
        unsigned atlasY{ 2 };

        unsigned maxChunkInstances{ 32 };

        std::string title{ "VoxelCraft" };
    };

    Renderer(Config config);

    void SetVP(const glm::mat4& vp);

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
    std::vector<std::reference_wrapper<const Renderable>> m_renderables;

    struct Buffers
    {
        unsigned vao;
        unsigned model;
        unsigned tex;
        unsigned pos;
    };
    Buffers m_buffer;

    PosBuffer m_posBuf;
    TexBuffer m_texBuf;

    sf::Window m_window;
    std::unique_ptr<Shader> m_shad;
};