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
        // Normal, Pos, UV

        // Back
         0.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Front
         1.f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         1.f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         1.f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         1.f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         1.f, -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         1.f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Left
         2.f, -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         2.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         2.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         2.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         2.f, -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         2.f, -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         
        // Right
         3.f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         3.f, 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         3.f, 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         3.f, 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         3.f, 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         3.f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        // Bottom
         4.f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         4.f,  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         4.f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         4.f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         4.f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         4.f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // Top
         5.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         5.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         5.f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         5.f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         5.f, -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         5.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    m_shad = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
    m_shad->Use();
    m_shad->SetMatrix("aMVP", glm::value_ptr(glm::mat4(1)));
    float atlasSize[2] = { 2,2 };
    m_shad->SetVec2("aAtlasSize", atlasSize);

    unsigned tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Atm you can see other texture edges so disabled for now
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
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
        
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribDivisor(0, 0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(1 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 0);
    }

    // Setup pos attrib
    {
        glGenBuffers(1, &m_buffer.pos);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.pos);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);
    }

    // Setup mesh tex
    {
        glGenBuffers(1, &m_buffer.tex);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.tex);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));

        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
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
        // Keep the buff for debug purposes
        const auto& posBuff = obj.get().GetPosData();
        {
            pos.insert(
                std::end(pos),
                std::begin(posBuff),
                std::end(posBuff));
        }

        const auto& texBuff = obj.get().GetTextureData();
        {
            tex.insert(
                std::end(tex),
                std::begin(texBuff),
                std::end(texBuff));
        }
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