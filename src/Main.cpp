#include "vcpch.hpp"

#include "common/Engine.hpp"
#include "common/Player.hpp"

#include "client/BlockData.hpp"
#include "client/Chunk.hpp"
#include "client/ChunkManager.hpp"

#include "client/gl/Camera.hpp"
#include "client/gl/Window.hpp"
#include "client/gl/ChunkRenderer.hpp"
#include "client/gl/PlayerRenderer.hpp"

//#include <SFML/Window/Window.hpp>
#include <iostream>

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

void LoadTexture(unsigned atlasX, unsigned atlasY)
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
        for (unsigned y = 0; y < height / texSize; y++)
        {
            for (unsigned x = 0; x < width / texSize; x++)
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

        const auto size = atlasX * atlasY;
        // Create the storage
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGBA8, texSize, texSize, size);

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, 0,
            texSize, texSize, size,
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

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

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
            m_camera->m_eye = glm::vec3(0,20,5);
            m_camera->m_target = glm::vec3(0,20,-1);
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
        // TODO move this to its own class
        // At the momement I'm planning on putting all the textures in the array texture
        // so I can put it here for the time being
        LoadTexture(atlasX, atlasY);

        // Create chunk manager
        m_chunkManager = std::make_unique<ChunkManager>(m_chunkRenderer);
        m_chunkManager->SetRadius(10 * chunkDimension.x);
        
        m_playerMesh = std::make_unique<PlayerMesh>(m_player.m_transform);

        std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
    }

    virtual void OnUpdate(Time dt) override final
    {
        m_camera->Update(dt);
        const glm::fvec3 camPos = m_camera->m_eye;
        
        // Process events
        // We may want to forward these events
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
