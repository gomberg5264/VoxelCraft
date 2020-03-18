#include "vcpch.hpp"

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

Renderer::Renderer(Config config)
    : m_instanceBuffer(config.maxChunkInstances * chunkSize)
{
    printf("Constructing renderer...\n");

    // ---
    // Create the main window
    {
        sf::ContextSettings contextSettings;
        contextSettings.depthBits = 24;
        contextSettings.majorVersion = 4;
        contextSettings.minorVersion = 5;
        contextSettings.attributeFlags |= sf::ContextSettings::Attribute::Debug;

        m_window.create(sf::VideoMode(config.x, config.y), config.title.c_str(), sf::Style::Default, contextSettings);
        m_window.setActive();
        printf("SFML OpenGL %d.%d\n", m_window.getSettings().majorVersion, m_window.getSettings().minorVersion);

        // Load OpenGL functions
        if (!gladLoadGL()) {
            throw "Glad couldn't be loaded!";
        }
        printf("Glad OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

        // Configure OpenGL
        glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
        glClearColor(0.2f, 0.f, 0.2f, 1.f);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Test error message!");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    // ---
    // Upload data to gpu
    {
        constexpr GLfloat cube[] =
        {
        // Normal, Pos, UV

        // Back
         0.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

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
         3.f, 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         3.f, 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         3.f, 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         3.f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         3.f, 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

        // Bottom
         4.f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         4.f,  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         4.f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         4.f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         4.f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         4.f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // Top
         5.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         5.f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         5.f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         5.f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         5.f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         5.f, -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };

        m_shad = std::make_unique<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
        m_shad->Use();
        
        // Load and generate the texture
        {
            unsigned tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
            
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            
            // Since we put our textures in a grid like pattern, we have to convert the data
            // to be in a format that emulates all the textures in one columns
            constexpr unsigned texSize = 64;
            unsigned char* data = stbi_load("res/texture.png", &width, &height, &nrChannels, STBI_rgb_alpha);

            if (data)
            {
                auto image{ std::make_unique<unsigned char[]>(width * height * nrChannels) };

                unsigned offset = 0;
                for (unsigned y = 0; y < height/texSize; y++)
                {
                    for (unsigned x = 0; x < width/texSize; x++)
                    {
                        // Read 64 rows
                        for (unsigned row = 0; row < texSize; row++)
                        {
                            memcpy(
                                &image[offset],
                                &data
                                [(y * width * texSize * nrChannels) + 
                                 (x * texSize * nrChannels) + 
                                  row * width * nrChannels],
                                texSize * nrChannels);
                            offset += texSize * nrChannels;
                        }
                    }
                }

                const auto size = config.atlasX * config.atlasY;
                // Create the storage
                glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGBA8, texSize, texSize, size);
                
                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0,
                    0,0,0,
                    texSize,texSize,size,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    image.get());

                glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
            
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // Set up vertex buffers and attributes
        {
            glGenVertexArrays(1, &m_buffer.vao);
            glBindVertexArray(m_buffer.vao);
            // Setup mesh attrib
            {
                glGenBuffers(1, &m_buffer.model);
                glBindBuffer(GL_ARRAY_BUFFER, m_buffer.model);
                glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
                
                // Normal
                glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
                glEnableVertexAttribArray(0);
                glVertexAttribDivisor(0, 0);

                // Pos
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(1 * sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
                glVertexAttribDivisor(1, 0);

                // UV
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
                glEnableVertexAttribArray(2);
                glVertexAttribDivisor(2, 0);
            }

            // Setup instancing attrib
            {
                glGenBuffers(1, &m_buffer.instance);
                glBindBuffer(GL_ARRAY_BUFFER, m_buffer.instance);
                
                // PosOffset
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
                // TexIndices
                glVertexAttribPointer(4, 3, GL_INT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                glVertexAttribPointer(5, 3, GL_INT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));                

                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
                glVertexAttribDivisor(3, 1);// These ones are prob redundant
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1); 
            }
        }
    }

    printf("Constructing renderer DONE\n");
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
    //unsigned drawCount = 0;
    //for (const auto& obj : m_renderables)
    //{
    //    drawCount += obj.get().GetDrawCount();
    //}

    // Fill Instance data
    m_instanceBuffer.SetActiveSize(0);
    for (const auto& obj : m_renderables)
    {
        const auto& drawData = obj.get().GetDrawData();
        
        memcpy(
            &m_instanceBuffer[m_instanceBuffer.GetActiveSize()],
            drawData.Data(), drawData.GetActiveSize() * sizeof(VBO::Vertex));
        
        m_instanceBuffer.SetActiveSize(
            m_instanceBuffer.GetActiveSize() + drawData.GetActiveSize());
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.instance);
    glBufferData(GL_ARRAY_BUFFER, 
        m_instanceBuffer.GetActiveSize() * sizeof(VBO::Vertex), 
        m_instanceBuffer.Data(), 
        GL_DYNAMIC_DRAW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, m_instanceBuffer.GetActiveSize());
    
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