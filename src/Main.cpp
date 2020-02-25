#include "vcpch.hpp"

class Game : public Engine
{
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

        mvp = project * view * model;
    }

    virtual void OnRender(Renderer &renderer) override final
    {
        renderer.SetMVP(mvp);

        // Render MVP
        //glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glm::mat4 mvp;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
