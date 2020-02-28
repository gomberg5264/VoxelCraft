#include "vcpch.hpp"

Renderer::Renderer(Config config)
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

    // ---
    // Upload data to gpu
    static constexpr GLfloat cube[] =
    {
        // positions    // colors (r, g, b, a)
        -0.5f, -0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f, -0.5f,  0.5f,  0, 0, 1, 1,
        -0.5f, -0.5f,  0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 0, 1, 1,

         0.5f, -0.5f, -0.5f,  0, 1, 0, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  0, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  0, 1, 0, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 0, 1,
         0.5f,  0.5f,  0.5f,  0, 1, 0, 1,

        -0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
        -0.5f, -0.5f,  0.5f,  1, 0, 0, 1,
        -0.5f, -0.5f,  0.5f,  1, 0, 0, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 0, 0, 1,

        -0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 1, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 1, 1, 1,
         0.5f,  0.5f, -0.5f,  0, 1, 1, 1,
         0.5f,  0.5f,  0.5f,  0, 1, 1, 1,

        -0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  1, 0, 1, 1,
        -0.5f,  0.5f, -0.5f,  1, 0, 1, 1,
         0.5f, -0.5f, -0.5f,  1, 0, 1, 1,
         0.5f,  0.5f, -0.5f,  1, 0, 1, 1,

        -0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
        -0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
        -0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
         0.5f, -0.5f,  0.5f,  1, 1, 0, 1,
         0.5f,  0.5f,  0.5f,  1, 1, 0, 1,
    };

    m_shad = std::make_unique<Shader>("res/shaders/default.vs", "res/shaders/default.fs");
    m_shad->Use();
    m_shad->SetMatrix("aMVP", glm::value_ptr(glm::mat4(1)));

    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        unsigned vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    }
}

void Renderer::SetVP(const glm::mat4& vp)
{
    m_vp = vp;
}

void Renderer::Render(const Renderable& renderable)
{
    m_renderables.push_back(renderable);
}

void Renderer::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    for (const auto& obj : m_renderables)
    {
        m_shad->SetMatrix("aMVP", glm::value_ptr(m_vp * obj.get().GetToWorld()));

        // Assumes only cubes
        obj.get().OnRender();
    }
    
    m_renderables.clear();

    m_window.display();
}

sf::Vector2u Renderer::GetSize() const
{
    return m_window.getSize();
}

sf::Window& Renderer::GetWindow()
{
    return m_window;
}

void Renderable::Render(Renderer& renderer) const
{
    renderer.Render(*this);
}

void CubeRender::OnRender() const
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
