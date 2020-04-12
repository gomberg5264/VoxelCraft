#include "vcpch.hpp"

#include "utils/Math.hpp"

#include "common/Engine.hpp"
#include "common/Player.hpp"

#include "common/BlockData.hpp"
#include "common/Chunk.hpp"

#include "client/ChunkManager.hpp"
#include "client/Camera.hpp"

#include "client/gl/Texture.hpp"
#include "client/gl/Window.hpp"
#include "client/gl/ChunkRenderer.hpp"
#include "client/gl/PlayerRenderer.hpp"

void RegisterBlockTypes(unsigned atlasX, unsigned atlasY)
{
    // Register block types
    // This function converts an x and y coordinate to an index
    auto T = [atlasX, atlasY](unsigned x, unsigned y)
    {
        return x + (atlasY - 1 - y) * atlasX;
    };

    // Register block data
    // ---
    // TODO Move this to a function or to be implemented in scripting
    {
        BlockData block;
        block.isSolid = false;
        BlockDataFactory::GetInstance().AddBlockData(BlockType::Air, block);
    }
    {
        BlockData block;
        block.SetSideUpBottomTexture(
            T(1, 1),
            T(0, 0),
            T(0, 1));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Grass, block);
    }
    {
        BlockData block;
        block.SetTexture(T(0, 1));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Dirt, block);
    }
    {
        BlockData block;
        block.SetTexture(T(1, 0));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Stone, block);
    }
}

// TODO: This is a client engine instance. Move it to a dedicated file
class Game : public Engine
{
private:
    virtual void OnInit() override final
    {
        sf::Clock time;
        // Setup camera
        // ---
        {
            m_camera = std::make_unique<FreelookCamera>(m_window.GetWindow());
            m_camera->m_eye = glm::vec3(0,20,10);
            m_camera->m_target = m_camera->m_eye + Transform::Forward();
            auto* cast = static_cast<FreelookCamera*>(m_camera.get());
            cast->m_speed = 5.f;
            cast->m_sensitivity = 0.2f;
        }
        
        // Setup texture atlas
        // ---
        const auto atlasX = 2;
        const auto atlasY = 2;
        RegisterBlockTypes(atlasX, atlasY);

        // Load and generate the texture
        // ---
        static TextureAtlas at("res/texture.png", atlasX, atlasY);
        //LoadTexture(atlasX, atlasY);

        // Create chunk manager
        m_chunkManager = std::make_unique<ChunkManager>(m_chunkRenderer);
        m_chunkManager->SetRadius(10.f * chunkDimension.x);
        
        m_chunkRenderer.SetSkyLightColor(glm::vec3(1));
        m_chunkRenderer.SetSkyLightDirection(glm::normalize(glm::vec3(-1, -3, -2)));
        m_chunkRenderer.SetSkyIntensity(0.6f);

        m_playerMesh = std::make_unique<PlayerMesh>(m_player.m_transform);

        std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
    }

    virtual void OnUpdate(Time dt) override final
    {
        m_camera->Update(dt);
        const glm::fvec3 camPos = m_camera->m_eye;
        
        // Process events
        // TODO: We may want to forward these events
        sf::Event event;
        while (m_window.GetWindow().pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                m_window.GetWindow().close();
                Stop();
            }
        
            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                m_window.GetWindow().close();
                Stop();
            }
        
            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        //std::printf("Cam x:%.2f y:%.2f z:%.2f\n",
        //    camPos.x,
        //    camPos.y,
        //    camPos.z);
        static float et = 0;
        et += dt;

        // TODO: temp day cycle
        constexpr float dayDur = 1.f / 0.1f;
        const float time = glm::radians(et * glm::pi<float>() * 2.f * dayDur);
        float tval = Math::InverseLerp(glm::cos(time), -1.f, 1.f);

        m_chunkRenderer.SetSkyIntensity(Math::Lerp(tval,0.3f,0.8f));
        m_chunkRenderer.SetSkyLightDirection(glm::vec3(
            glm::cos(time),
            glm::cos(time),
            glm::cos(time)));

        // TODO: Move this to a player class
        m_player.m_transform.m_pos = glm::vec3(0, 20, 0);
        m_player.m_transform.m_scale = glm::vec3(glm::sin(et),glm::cos(et),glm::sin(glm::cos(et))) * 10.f;
        m_player.m_transform.m_euler = m_player.m_transform.m_euler + glm::vec3(dt, dt * 3.f, dt * 1.5f);

        m_chunkManager->SetPos(m_camera->m_eye);
        m_chunkManager->Update();

        m_chunkRenderer.SetVP(m_camera->GetProjection() * m_camera->GetView());
        m_window.Clear();

        m_chunkManager->Render();
        m_chunkRenderer.Display();

        m_playerMesh->Draw(*m_camera);

        m_window.Display();
    }

    Window m_window;
    std::unique_ptr<ChunkManager> m_chunkManager;
    ChunkRenderer m_chunkRenderer;

    Player m_player;
    std::unique_ptr<PlayerMesh> m_playerMesh;

    std::unique_ptr<Camera> m_camera;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
