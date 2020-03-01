#pragma once
#include <vcpch.cpp>

template <typename T>
struct Texture
{
    using UV = std::pair<T, T>;

    enum Side
    {
        Top,
        Left,
        Back,
        Right,
        Front,
        Bottom
    };

    void SetSide(const UV& uv)
    {
        this->uv[Left] = this->uv[Back] = this->uv[Right] = this->uv[Front] = uv;
    }

    UV uv[6];
};

class TextureAtlas
{
public:
    TextureAtlas(unsigned width, unsigned heigth);
    
    void AddTexture(Block::Type type, const Texture<unsigned>& texture);
    const Texture<float>& GetTexture(Block::Type type) const;

private:
    std::unordered_map<Block::Type, Texture<float>> m_textures;
    const float m_width;
    const float m_height;
};