#include "vcpch.hpp"
#include <iostream>

class Game : public Engine
{
public:
    Game(Config config = {})
        : Engine(config)
    {}

private:
    virtual void OnInit(Renderer& renderer) override final
    {
        sf::Clock time;
        
        // Setup camera
        m_camera = std::make_unique<FreelookCamera>(renderer.GetWindow());
        m_camera->m_eye = glm::vec3(0,0,6);
        m_camera->m_target = glm::vec3(0,0,0);
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
        
        const auto atlasX = m_config.graphics.atlasX;
        const auto atlasY = m_config.graphics.atlasY;
        // Register block types
        auto T = [atlasX,atlasY](unsigned x, unsigned y)
        {
            return x + (atlasY - 1 -y) * atlasX; 
        };
        BlockDataFactory bData;
        {
            BlockData block;
            block.isSolid = false;
            bData.AddBlockData(BlockType::Air,block);
        }
        {
            BlockData block;
            block.SetSideUpBottomTexture(
                T(1, 1),
                T(0, 0), 
                T(0, 1));

            bData.AddBlockData(BlockType::Grass,block);
        }
        {
            BlockData block;
            block.SetTexture(T(0, 1));

            bData.AddBlockData(BlockType::Dirt, block);
        }
        {
            BlockData block;            
            block.SetTexture(T(1, 0));

            bData.AddBlockData(BlockType::Stone, block);
        }

        //Generate voxels
        m_chunkManager = std::make_unique<ChunkManager>(bData, m_config.graphics.chunkRenderRadius);
        
        // TEMP
        m_chunkManager->Update(); 

        //constexpr int size = 8;
        //for (int x = 0; x < size; x++)
        //{
        //    for (int z = 0; z < size; z++)
        //    {
        //        m_chunks.push_back(std::make_unique<Chunk>(bData, glm::fvec3{ x * chunkDimension.x,0,z * chunkDimension.z }));
        //        auto& chunk = m_chunks.back();
        //        chunk->Generate();
        //    }
        //    std::cout << (x + 1) * size << '/' << size * size << '\n';
        //}

        std::printf("Init time: %.2f", time.getElapsedTime().asSeconds());
    }

    virtual void OnUpdate(Time dt) override final
    {
        m_camera->Update(dt);
        const glm::fvec3 camPos = m_camera->GetView()[3];
        
        //m_chunkManager.SetLoadPos(camPos);
        //m_chunkManager->Update();

        //std::printf("Cam x:%.2f y:%.2f z:%.2f\n",
        //    m_camera->GetView()[3].x,
        //    m_camera->GetView()[3].y,
        //    m_camera->GetView()[3].z);
    }

    virtual void OnRender(Renderer &renderer) override final
    {
        renderer.SetVP(m_camera->GetProjection() * m_camera->GetView());

        renderer.Render(*m_chunkManager);
        //for (const auto& chunk : m_chunks)
        //    renderer.Render(*chunk);
    }


    //std::vector<std::unique_ptr<Chunk>> m_chunks;
    std::unique_ptr<ChunkManager> m_chunkManager;

    std::unique_ptr<Camera> m_camera;
};

int main()
{
    Game::Config config;
    config.graphics.atlasX = 2;
    config.graphics.atlasY = 2;
    config.graphics.title = "VoxelCraft";
    config.graphics.chunkRenderRadius = 2;
    config.graphics.maxChunkInstances = 2 *2 *2 *2;

    Game game(config);
    game.Run();

    return EXIT_SUCCESS;
}
