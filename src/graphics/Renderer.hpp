#pragma once

class Renderer;

class Renderable
{
public:
    void Render(Renderer& renderer) const;

    // Gets called when rendering with bound buffer. Should call glDrawArrays
    virtual void OnRender() const = 0;

    virtual glm::mat4 GetToWorld() const = 0;
};

class CubeRender : public Renderable
{
    virtual void OnRender() const override final;
};

class Renderer
{
public:
    struct Config
    {
        float x{ 1280 };
        float y{ 720 };

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

    glm::mat4 m_vp;
    sf::Window m_window;
    std::unique_ptr<Shader> m_shad;
};