#pragma once

class TextureAtlas
{
public:
    TextureAtlas(unsigned width, unsigned heigth);
    
    void Initialize(const BlockMetaFactory& factory);
    
    void AddTexture(BlockType type, const Texture<unsigned>& texture);
    const Texture<float>& GetTexture(BlockType type) const;

private:
    std::unordered_map<BlockType, Texture<float>> m_textures;
    const float m_width;
    const float m_height;
};