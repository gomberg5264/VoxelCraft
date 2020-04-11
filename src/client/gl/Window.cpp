#include "vcpch.hpp"
#include "client/gl/Window.hpp"

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

Window::Window(Config config)
{
    printf("Constructing Window...\n");

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

        //glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
        //    GL_DEBUG_SEVERITY_MEDIUM, -1, "Test error message!");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    printf("Constructing Window DONE\n");
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Display()
{
    m_window.display();
}

sf::Vector2u Window::GetSize() const
{
    return m_window.getSize();
}

sf::Window& Window::GetWindow()
{
    return m_window;
}