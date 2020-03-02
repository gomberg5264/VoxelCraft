#include "vcpch.hpp"

TextureAtlas* atlas = nullptr;

class Cube : public Renderable
{
public:
    Cube(const glm::fvec3& pos, const BlockMeta& meta)
    {
        m_pos.push_back(pos.x);
        m_pos.push_back(pos.y);
        m_pos.push_back(pos.z);

        const auto& tex = (*atlas).GetTexture(meta.type).uv;
        for (int i = 0; i < 6; i++)
        {
            // This way, every vertex has the uv specified
            // This means that we are wasting 6* as much space but I don't know
            // how to tell the vertex shader to only continue 

            // We kinda fake instance it
            //for (int j = 0; j < 6; j++)
            {
                m_texture.push_back(tex[i].first);
                m_texture.push_back(tex[i].second);
            }
        }
    }

    virtual const std::vector<GLfloat>& GetPosData() const override final
    {
        return m_pos;
    }

    virtual const std::vector<GLfloat>& GetTextureData() const override final
    {
        return m_texture;
    }
private:

    std::vector<GLfloat> m_pos;
    std::vector<GLfloat> m_texture;
};

class Game : public Engine
{
private:
    virtual void OnInit(Renderer& renderer) override final
    {
        // Setup camera
        m_camera = std::make_unique<FreelookCamera>(renderer.GetWindow());
        m_camera->m_eye = glm::vec3(0,0,6);
        m_camera->m_target = glm::vec3(0,0,0);
        auto* cast = static_cast<FreelookCamera*>(m_camera.get());
        cast->m_speed = 5.f;
        cast->m_sensitivity = 0.2f;
        
        // Register block types
        BlockMetaFactory meta;
        {
            BlockMeta block;
            block.type = BlockType::Grass;
            block.texture.uv[TextureFace::Top] = { 0,0 };
            block.texture.uv[TextureFace::Bottom] = { 0,1 };
            block.texture.SetSide({ 1,1 });

            meta.AddBlockMeta(block);
        }
        // Stone
        {
            BlockMeta block;
            block.type = BlockType::Stone;
            block.texture.SetBlock({ 1,0 });

            meta.AddBlockMeta(block);
        }

        renderer.m_textureAtlas.Initialize(meta);
        atlas = &renderer.m_textureAtlas;


        //m_cubes.emplace_back(
        //    glm::fvec3(0.5f, 0.5f, 0.5f),
        //    meta.GetBlockMeta(BlockType::Stone));

        //return;

        // Generate some voxels
        constexpr int size = 2; // Actually radius

        //for (int y = -size; y < size; y++)
        //{
        //    glm::fvec3 o(0, y, 0);

        //    BlockMeta bmeta;
        //    if (y >= 0)
        //    {
        //        bmeta = meta.GetBlockMeta(BlockType::Grass);
        //    }
        //    else
        //    {
        //        bmeta = meta.GetBlockMeta(BlockType::Stone);
        //    }

        //    m_cubes.emplace_back(
        //        glm::fvec3(o.x + 0.5f, o.y + 0.5f, o.z + 0.5f),
        //        bmeta);
        //}

        //return;

        for (int x = -size; x < size; x++)
        {
            for (int y = -size; y < size; y++)
            {
                for (int z = -size; z < size; z++)
                {
                    glm::fvec3 o(x, y, z);
                    
                    BlockMeta bmeta;
                    if (y >= 0)
                    {
                        bmeta = meta.GetBlockMeta(BlockType::Grass);
                    }
                    else
                    {
                        bmeta = meta.GetBlockMeta(BlockType::Stone);
                    }

                    m_cubes.emplace_back(
                        glm::fvec3(o.x + 0.5f, o.y + 0.5f, o.z + 0.5f),
                        bmeta);
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
