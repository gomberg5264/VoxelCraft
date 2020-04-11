#include "vcpch.hpp"
#include "common/BlockData.hpp"

#include "client/gl/Primitive.hpp"

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

std::vector<unsigned> Primitive::Face::MakeIndices(unsigned faceCount)
{
    const auto elementCount = faceCount * 6u;
    //unsigned* ebo = new unsigned[elementCount];
    std::vector<unsigned> ebo;
    unsigned offset = 0;
    for (unsigned i = 0; i < elementCount; i += 6)
    {
        std::vector<unsigned> indices { 3 + offset,0 + offset,1 + offset,3 + offset,1 + offset,2 + offset };
        ebo.insert(ebo.end(), indices.begin(), indices.end());
        //memcpy(&ebo[i], indices, 6 * sizeof(unsigned));
        offset += 4;
    }

    return ebo;
}
