#include "vcpch.hpp"

TextureAtlas::TextureAtlas(unsigned width, unsigned height)
    : m_width(width)
    , m_height(height)
{
}

void TextureAtlas::AddTexture(BlockMeta meta, const Texture<unsigned>& texture)
{
    // Convert to uv
    Texture<float> uv;
    for (int i = 0; i < 6; i++)
    {
        uv.uv[i].first = static_cast<float>(texture.uv[i].first) / m_width;
        uv.uv[i].second = static_cast<float>(texture.uv[i].second) / m_height;
    }

    m_textures[meta] = uv;
}

const Texture<float>& TextureAtlas::GetTexture(BlockType type) const
{
    return m_textures.at(type);
}
