#include "vcpch.hpp"

// Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
constexpr GLfloat cube[] =
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


constexpr GLfloat face[] =
{
    -0.5,-0.5,0, 1,0,0,1,
     0.5,-0.5,0, 0,1,0,1,
     0,0.5,0,    0,0,1,1
};

int main()
{
    {
        sf::Context context;
    
        if (!gladLoadGL()) {
            printf("Glad couldn't be loaded!\n");
            exit(-1);
        }
        printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    }
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;

    // Create the main window
    sf::Window window(sf::VideoMode(640, 480), "SFML window with OpenGL", sf::Style::Default, contextSettings);

    // Make it the active window for OpenGL calls
    window.setActive();

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    glClearColor(0.2f, 0.f, 0.2f, 1.f);

    // Create vao
    Shader shad("res/shaders/default.vs","res/shaders/default.fs");
    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        unsigned vbo; 
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    // Create a clock for measuring the time elapsed
    sf::Clock clock;
    shad.Use();
    glBindVertexArray(vao);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations to rotate the cube
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
        //glTranslatef(0.f, 0.f, -200.f);
        //glRotatef(clock.getElapsedTime().asSeconds() * 50, 1.f, 0.f, 0.f);
        //glRotatef(clock.getElapsedTime().asSeconds() * 30, 0.f, 1.f, 0.f);
        //glRotatef(clock.getElapsedTime().asSeconds() * 90, 0.f, 0.f, 1.f);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Finally, display the rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
