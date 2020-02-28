#include "vcpch.hpp"

class Cube : public CubeRender
{
public:
    virtual glm::mat4 GetToWorld() const
    {
        return m_toWorld;
    }

    void Update(float dt)
    {
        m_dt += dt;

        auto rot = glm::mat4(1);
        rot = glm::rotate(rot, glm::radians(m_dt * 50), { 1.f, 0.f, 0.f });
        rot = glm::rotate(rot, glm::radians(m_dt * 30), { 0.f, 1.f, 0.f });
        rot = glm::rotate(rot, glm::radians(m_dt * 90), { 0.f, 0.f, 1.f });
    
        m_toWorld = glm::scale(glm::mat4(1), glm::vec3(m_scale));
        m_toWorld = rot * m_toWorld;
        m_toWorld = glm::translate(glm::mat4(1), m_offset * m_scale) * m_toWorld;
    }


    float m_dt{ 0 };
    float m_scale;
    glm::vec3 m_offset;
    glm::mat4 m_toWorld{ 1 };
};

class Game : public Engine
{
private:
    virtual void OnInit()
    {
        constexpr int size = 2;

        for (float x = -size; x < size; x++)
        {
            for (float y = -size; y < size; y++)
            {
                for (float z = -size; z < size; z++)
                {
                    m_cubes.emplace_back();
                    m_cubes.back().m_offset = glm::fvec3(x*2 + 0.5f, y*2 + 0.5f, (z + size) * 2);
                    m_cubes.back().m_scale = 1.f / (size * size * size);
                }
            }
        }
    }

    virtual void OnUpdate(Time dt) override final
    {
        // Calculate models
        for (auto& cube : m_cubes)
        {
            cube.Update(dt);
        }
    }

    virtual void OnRender(Renderer &renderer) override final
    {
        const auto project = glm::perspective(glm::radians(45.f), (float)GetWindowSize().x / (float)GetWindowSize().y, 0.1f, 100.f);
        const auto view = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f));
        renderer.SetVP(project * view);

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
