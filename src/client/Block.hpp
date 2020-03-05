#pragma once

class Block : public Renderable
{
public:
    Block() : m_pos(1), m_texture(1) {}

    void Init(const glm::fvec3& pos, const BlockData& meta, const TextureAtlas& atlas)
    {
        m_pos.CopyFrom(glm::value_ptr(pos), 0, 3);
        //m_pos.push_back(pos.x);
        //m_pos.push_back(pos.y);
        //m_pos.push_back(pos.z);

        const auto& tex = atlas.GetTexture(meta.type).uv;
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
            m_texture.Data()[j++] = (tex[i].first);
            m_texture.Data()[j++] = (tex[i].second);
        }
    }

    virtual const Buffer& GetPosData() const noexcept override final
    {
        return m_pos;
    }

    virtual const Buffer& GetTextureData() const noexcept override final
    {
        return m_texture;
    }

    const BlockData& GetData() const noexcept
    {
        return m_data;
    }

private:

    BlockData m_data;
    PosBuffer m_pos;
    TexBuffer m_texture;
};