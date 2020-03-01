#pragma once

/**
 * Used to index in the texture atlas
 * Needed by both texture atlas and block
 */

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