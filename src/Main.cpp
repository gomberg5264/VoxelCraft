#include "vcpch.hpp"

class Cube : public Renderable
{
public:
    Cube(const glm::fvec3& pos)
    {
        m_drawData.push_back(pos.x);
        m_drawData.push_back(pos.y);
        m_drawData.push_back(pos.z);
    }

    virtual const std::vector<GLfloat>& GetDrawData() const override final
    {
        return m_drawData;
    }

    std::vector<GLfloat> m_drawData;
};

class Game : public Engine
{
private:
    virtual void OnInit(Renderer& renderer) override final
    {
        m_camera = std::make_unique<FreelookCamera>(renderer.GetWindow());
        m_camera->m_eye = glm::vec3(0,0,6);
        m_camera->m_target = glm::vec3(0,0,0);
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
        
        constexpr int size = 4;

        for (int x = -size; x < size; x++)
        {
            for (int y = -size; y < size; y++)
            {
                for (int z = -size; z < size; z++)
                {
                    glm::fvec3 o(x, y, z);
                    m_cubes.emplace_back(glm::fvec3(o.x + 0.5f, o.y + 0.5f, o.z + 0.5f));
                }
            }
        }
    }

    virtual void OnUpdate(Time dt) override final
    {
        m_camera->Update(dt);

    }

    virtual void OnRender(Renderer &renderer) override final
    {
        renderer.SetVP(m_camera->GetProjection() * m_camera->GetView());

        for (const auto& cube : m_cubes)
            renderer.Render(cube);
    }

    std::vector<Cube> m_cubes;

    std::unique_ptr<Camera> m_camera;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
