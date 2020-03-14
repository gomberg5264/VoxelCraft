#include "vcpch.hpp"


class Game : public Engine
{
public:
    Game(Config config = {})
        : Engine(config) {}

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
        
        // Register block types

        BlockDataFactory meta;
        {
            BlockData block;
            block.type = BlockType::Air;
            block.isSolid = false;
            meta.AddBlockData(block);
        }
        {
            BlockData block;
            block.type = BlockType::Grass;
            block.texture.uv[TextureFace::Top] = { 0,0 };
            block.texture.uv[TextureFace::Bottom] = { 0,1 };
            block.texture.SetSide({ 1,1 });

            meta.AddBlockData(block);
        }
        {
            BlockData block;
            block.type = BlockType::Dirt;
            //block.texture.uv[TextureFace::Top] = { 0,0 };
            //block.texture.uv[TextureFace::Bottom] = { 0,1 };
            block.texture.SetBlock({ 0,1 });

            meta.AddBlockData(block);
        }
        // Stone
        {
            BlockData block;
            block.type = BlockType::Stone;
            block.texture.SetBlock({ 1,0 });

            meta.AddBlockData(block);
        }

        TextureAtlas atlas(2,2);
        atlas.Initialize(meta);

        //Generate some voxels
        constexpr int size = 2;
        for (int x = 0; x < size; x++)
        {
            for (int z = 0; z < size; z++)
            {
                m_chunks.push_back(std::make_unique<Chunk>());
                auto& chunk = m_chunks.back();
                chunk->SetPos({ x * float(chunkDimension.x), -float(chunkDimension.y) * 0.5f, z * -float(chunkDimension.z)});
                chunk->Generate(meta,atlas);
            }
            std::cout << (x + 1) * 8 << '/' << 8 * 8 << '\n';
        }

        std::printf("Init time: %.2f", time.getElapsedTime().asSeconds());
    }

    virtual void OnUpdate(Time dt) override final
    {
        m_camera->Update(dt);

    }

    virtual void OnRender(Renderer &renderer) override final
    {
        renderer.SetVP(m_camera->GetProjection() * m_camera->GetView());

        for (const auto& chunk : m_chunks)
            renderer.Render(*chunk);
    }

    std::vector<std::unique_ptr<Chunk>> m_chunks;

    std::unique_ptr<Camera> m_camera;
};

int main()
{
    Game::Config config;
    config.graphics.atlasX = 2;
    config.graphics.atlasY = 2;
    config.graphics.title = "VoxelCraft";
    config.graphics.maxChunkInstances = 64;

    Game game(config);
    game.Run();

    return EXIT_SUCCESS;
}
