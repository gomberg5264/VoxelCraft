#include "vcpch.hpp"

// Define a 3D cube (6 facesmade of 2 triangles composed by 3 vertices)
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


class Game : public Engine
{
public:
    Game()
        : m_shad("res/shaders/default.vs", "res/shaders/default.fs")
    {
        // Create vao
        Shader shad("res/shaders/default.vs", "res/shaders/default.fs");
        unsigned vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        {
            unsigned vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }

        // Create a clock for measuring the time elapsed
        shad.Use();
    }

private:
    virtual void OnUpdate(Time dt) override final
    {
        static Time elapsed = 0;
        elapsed += dt;

        // Calculate MVP
        const auto project = glm::perspective(glm::radians(45.f), (float)GetWindowSize().x / (float)GetWindowSize().y, 0.1f, 100.f);
        const auto view = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f));
        auto model = glm::rotate(glm::mat4(1), glm::radians(elapsed * 50), { 1.f, 0.f, 0.f });
        model = glm::rotate(model, glm::radians(elapsed * 30), { 0.f, 1.f, 0.f });
        model = glm::rotate(model, glm::radians(elapsed * 90), { 0.f, 0.f, 1.f });

        // Set MVP
        m_shad.SetMatrix("aMVP", glm::value_ptr(project * view * model));
    }

    virtual void OnRender() override final
    {
        // Render MVP
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    Shader m_shad;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
