#include "vcpch.hpp"
#include <iostream>

class Game : public Engine
{
public:
    Game() 
        : m_chunkRenderer(1)
        , m_mesh(0)
    {}
private:
    virtual void OnInit() override final
    {
        sf::Clock time;
        
        // Setup camera
        m_camera = std::make_unique<FreelookCamera>(m_window.GetWindow());
        m_camera->m_eye = glm::vec3(0,0,5);
        m_camera->m_target = glm::vec3(0,0,-1);
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
        
        // Setup atlas
        const auto atlasX = 2;
        const auto atlasY = 2;
        // Register block types
        auto T = [atlasX,atlasY](unsigned x, unsigned y)
        {
            return x + (atlasY - 1 -y) * atlasX; 
        };

        // Make sure that this stays alive 
        //static BlockDataFactory bData;
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

        m_chunk = std::make_unique<Chunk>(glm::ivec3(0,0,-1));
        m_chunk->Generate();
        m_mesh.Generate(*m_chunk.get());

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

        switch (m_chunk->GetState())
        {
        case Chunk::State::New:
            m_chunkRenderer.Render(m_mesh, true);
            m_chunk->MarkDone();
            break;
        case Chunk::State::Modified:
            m_chunkRenderer.Render(m_mesh, true);
            m_chunk->MarkDone();
            break;
        case Chunk::State::Done:
            m_chunkRenderer.Render(m_mesh,false);
            break;
        }

        m_chunkRenderer.SetVP(m_camera->GetProjection() * m_camera->GetView());
        //m_chunkRenderer.SetVP(glm::mat4(1));

        m_window.Clear();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_chunkRenderer.Display();

        m_window.Display();
    }

    Window m_window;

    ChunkRenderer m_chunkRenderer;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Chunk> m_chunk;
    ChunkMesh m_mesh;
};

int main()
{
    Game game;
    game.Run();

    return EXIT_SUCCESS;
}
