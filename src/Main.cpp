#include "vcpch.hpp"

class Cube : public CubeRender
{
public:
    virtual glm::mat4 GetToWorld() const
    {
        return m_toWorld;
    }

    glm::mat4 m_toWorld{ 1 };
};

class Game : public Engine
{
private:
    virtual void OnInit()
    {
        m_cubes.emplace_back();
        //m_cubes.back().m_toWorld = 
        return;

        for (int x = -16; x < 16; x++)
        {
            for (int y = -16; y < 16; y++)
            {
                for (int z = -16; z < 16; z++)
                {
                    m_cubes.emplace_back();
                    m_cubes.back().m_toWorld = glm::translate(glm::mat4(1), glm::fvec3(x, y, z + 15));
                }
            }
        }
    }

    virtual void OnUpdate(Time dt) override final
    {
        static Time elapsed = 0;
        elapsed += dt;

        // Calculate MVP
        const auto project = glm::perspective(glm::radians(45.f), (float)GetWindowSize().x / (float)GetWindowSize().y, 0.1f, 100.f);
        const auto view = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f));
        auto& model = m_cubes.back().m_toWorld;
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(elapsed * 50), { 1.f, 0.f, 0.f });
        model = glm::rotate(model, glm::radians(elapsed * 30), { 0.f, 1.f, 0.f });
        model = glm::rotate(model, glm::radians(elapsed * 90), { 0.f, 0.f, 1.f });

        m_vp = project * view;
    }

    virtual void OnRender(Renderer &renderer) override final
    {
        renderer.SetVP(m_vp);

        for (const auto& cube : m_cubes)
            cube.Render(renderer);

        // Render MVP
        //glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    std::vector<Cube> m_cubes;

    glm::mat4 m_vp;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
