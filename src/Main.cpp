#include "vcpch.hpp"
#include <iostream>

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
            m_camera->m_eye = glm::vec3(0,0,5);
            m_camera->m_target = glm::vec3(0,0,-1);
            auto* cast = static_cast<FreelookCamera*>(m_camera.get());
            cast->m_speed = 5.f;
            cast->m_sensitivity = 0.2f;
        }
        
        // Setup texture atlas
        // ---
        const auto atlasX = 2;
        const auto atlasY = 2;
        {
        // Register block types
        // This function converts an x and y coordinate to an index
        auto T = [atlasX,atlasY](unsigned x, unsigned y)
        {
            return x + (atlasY - 1 -y) * atlasX; 
        };

        // Register block data
        // ---
        // TODO Move this to a function or to be implemented in scripting
        {
            BlockData block;
            block.isSolid = false;
            BlockDataFactory::GetInstance().AddBlockData(BlockType::Air,block);
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

        // Load and generate the texture
        // ---
        // TODO move this to its own class
        // At the momement I'm planning on putting all the textures in the array texture
        // so I can put it here for the time being
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

        // TEMP
        // ---
        // Generate a chunk. This should all happen during runtime
        {
            m_chunkRenderer = std::make_unique<ChunkRenderer>();

            m_chunk = std::make_unique<Chunk>(glm::ivec3(0,0,-1));
            m_chunk->Generate();
            m_mesh = std::make_unique<ChunkMesh>();
            m_chunkRenderer->RegisterVAO(m_mesh->m_vao);
            m_mesh->Generate(*m_chunk);
            
            m_chunk2 = std::make_unique<Chunk>(glm::ivec3(chunkDimension.x, 0, -1));
            m_chunk2->Generate();
            m_mesh2 = std::make_unique<ChunkMesh>();
            m_chunkRenderer->RegisterVAO(m_mesh2->m_vao);
            m_mesh2->Generate(*m_chunk2);
        }

        std::printf("Init time: %.2f\n", time.getElapsedTime().asSeconds());
    }

    std::unique_ptr<ChunkRenderer> m_chunkRenderer;
    // temp
    std::unique_ptr<Chunk> m_chunk;
    std::unique_ptr<ChunkMesh> m_mesh;
    std::unique_ptr<Chunk> m_chunk2;
    std::unique_ptr<ChunkMesh> m_mesh2;


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

        switch (m_chunk->GetState())
        {
        case Chunk::State::New:
            m_chunkRenderer->Render(*m_mesh, true);
            m_chunk->MarkDone();
            break;
        case Chunk::State::Modify:
            m_chunkRenderer->Render(*m_mesh, true);
            m_chunk->MarkDone();
            break;
        case Chunk::State::Done:
            m_chunkRenderer->Render(*m_mesh,false);
            break;
        }

        switch (m_chunk2->GetState())
        {
        case Chunk::State::New:
            m_chunkRenderer->Render(*m_mesh2, true);
            m_chunk2->MarkDone();
            break;
        case Chunk::State::Modify:
            m_chunkRenderer->Render(*m_mesh2, true);
            m_chunk2->MarkDone();
            break;
        case Chunk::State::Done:
            m_chunkRenderer->Render(*m_mesh2, false);
            break;
        }

        m_chunkRenderer->SetVP(m_camera->GetProjection() * m_camera->GetView());
        m_window.Clear();

        m_chunkRenderer->Display();

        m_window.Display();
    }

    Window m_window;

    std::unique_ptr<Camera> m_camera;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
