#include "vcpch.hpp"

Primitive::Face::Buffer Primitive::Face::MakeBuffer(BlockFace dir, float x, float y, float z, unsigned texIndex)
{
    Buffer buffer;
    // Top left
    // Bottom left
    // Bottom right
    // Top right

    switch (dir)
    {
    case BlockFace::Top:
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 0.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, texIndex);

        break;
    case BlockFace::Bottom:
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 1.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Left:
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Right:
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, texIndex);

        break;
    case BlockFace::Back:
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Front:
        buffer.vertices.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, texIndex);
        buffer.vertices.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 1.f, 0.f, texIndex);
        buffer.vertices.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f, texIndex);

        break;
    }

    return buffer;
}
