#include "vcpch.hpp"

Renderer::Renderer(Config config)
    : m_textureAtlas(2,2)
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
        // Back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         
        // Right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // Bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // Top
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    m_shad = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
    m_shad->Use();
    m_shad->SetMatrix("aMVP", glm::value_ptr(glm::mat4(1)));
    float atlasSize[2] = { m_textureAtlas.GetSize().first, m_textureAtlas.GetSize().second };
    m_shad->SetVec2("aAtlasSize", atlasSize);

    unsigned tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/texture.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenVertexArrays(1, &m_buffer.vao);
    glBindVertexArray(m_buffer.vao);
    // Setup mesh attrib
    {
        glGenBuffers(1, &m_buffer.model);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.model);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribDivisor(0, 0);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 0);
    }

    // Setup mesh tex
    {
        glGenBuffers(1, &m_buffer.tex);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.tex);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 6);
    }

    // Setup pos attrib
    {
        glGenBuffers(1, &m_buffer.pos);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.pos);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
    }
}

void Renderer::SetVP(const glm::mat4& vp)
{
    m_shad->SetMatrix("aVP", glm::value_ptr(vp));
}

void Renderer::Render(const Renderable& renderable)
{
    m_renderables.push_back(renderable);
}

void Renderer::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // Fill pos data
    std::vector<GLfloat> pos;
    std::vector<GLfloat> tex;

    for (const auto& obj : m_renderables)
    {
        pos.insert(
            std::end(pos),
            std::begin(obj.get().GetPosData()),
            std::end(obj.get().GetPosData()));

        tex.insert(
            std::end(tex),
            std::begin(obj.get().GetTextureData()),
            std::end(obj.get().GetTextureData()));
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.pos);
    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(GLfloat), pos.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.tex);
    glBufferData(GL_ARRAY_BUFFER, tex.size() * sizeof(GLfloat), tex.data(), GL_DYNAMIC_DRAW);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, m_renderables.size());
    
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