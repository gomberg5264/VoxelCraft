#pragma once

/**
 * Used to index in the texture atlas
 * Needed by both texture atlas and block
 */

enum TextureFace
{
    Back,
    Front,
    Left,
    Right,
    Bottom,
    Top
};

template <typename T>
struct Texture
{
    using UV = std::pair<T, T>;

    void SetSide(const UV& uv)
    {
        this->uv[Left] = this->uv[Back] = this->uv[Right] = this->uv[Front] = uv;
    }

    void SetBlock(const UV& uv)
    {
        for (int i = 0; i < 6; i++)
            this->uv[i] = uv;
    }

    UV uv[6];
};