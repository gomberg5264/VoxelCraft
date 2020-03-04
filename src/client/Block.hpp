#pragma once

class Block : public Renderable
{
public:
    //Block(const glm::fvec3& pos, const BlockMeta& meta, const TextureAtlas& atlas)

    void Init(const glm::fvec3& pos, const BlockData& meta, const TextureAtlas& atlas)
    {
        m_pos.push_back(pos.x);
        m_pos.push_back(pos.y);
        m_pos.push_back(pos.z);

        const auto& tex = atlas.GetTexture(meta.type).uv;
        for (int i = 0; i < 6; i++)
        {
            m_texture.push_back(tex[i].first);
            m_texture.push_back(tex[i].second);
        }
    }

    virtual const std::vector<GLfloat>& GetPosData() const noexcept override final
    {
        return m_pos;
    }

    virtual const std::vector<GLfloat>& GetTextureData() const noexcept override final
    {
        return m_texture;
    }
private:

    std::vector<GLfloat> m_pos;
    std::vector<GLfloat> m_texture;
};