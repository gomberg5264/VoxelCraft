#include "vcpch.hpp"

TextureAtlas::TextureAtlas(unsigned width, unsigned height)
    : m_width(width)
    , m_height(height)
{
}

void TextureAtlas::Initialize(const BlockMetaFactory& factory)
{
    for (const auto& block : factory.GetData())
    {
        AddTexture(block.first, block.second.texture);
    }
}

void TextureAtlas::AddTexture(BlockType type, const Texture<unsigned>& texture)
{
    // Convert to uv
    Texture<float> uv;
    for (int i = 0; i < 6; i++)
    {
        uv.uv[i].first = static_cast<float>(texture.uv[i].first) / m_width;
        uv.uv[i].second = 1.f - static_cast<float>(texture.uv[i].second) / m_height;
    }

    m_textures[type] = uv;
}

const Texture<float>& TextureAtlas::GetTexture(BlockType type) const
{
    return m_textures.at(type);
}

std::pair<unsigned, unsigned> TextureAtlas::GetSize() const
{
    return std::pair<unsigned, unsigned>(m_width,m_height);
}
