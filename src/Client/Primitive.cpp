#include "Client/Primitive.h"
#include "Common/BlockData.h"

#include <vector>

Face::Vertices Face::CreateVertices(BlockFace dir, float x, float y, float z, unsigned texIndex)
{

    // Top left
    // Bottom left
    // Bottom right
    // Top right
    Face::Vertices buffer;

    switch (dir)
    {
    case BlockFace::Top:
        //                  x         y         z         nx   ny   nz   u    v    tIndex
        buffer.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, texIndex);
        buffer.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, 0.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 0.f, 1.f, 0.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 0.f, 1.f, 0.f, 1.f, 1.f, texIndex);

        break;
    case BlockFace::Bottom:
        buffer.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 0.f, -1.f, 0.f, 1.f, 1.f, texIndex);
        buffer.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 0.f, -1.f, 0.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 0.f, -1.f, 0.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 0.f, -1.f, 0.f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Left:
        buffer.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, -1.f, 0.f, 0.f, 1.f, 1.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, -1.f, 0.f, 0.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, -1.f, 0.f, 0.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, -1.f, 0.f, 0.f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Right:
        buffer.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f, texIndex);
        buffer.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 1.f, 0.f, 0.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 1.f, 0.f, 0.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 0.f, 0.f, 1.f, 1.f, texIndex);

        break;
    case BlockFace::Back:
        buffer.emplace_back(x + 0.5f, y + 0.5f, z - 0.5f, 0.f, 0.f, -1.f, 1.f, 1.f, texIndex);
        buffer.emplace_back(x + 0.5f, y - 0.5f, z - 0.5f, 0.f, 0.f, -1.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 0.f, 0.f, -1.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 0.f, 0.f, -1.f, 0.f, 1.f, texIndex);

        break;
    case BlockFace::Front:
        buffer.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, texIndex);
        buffer.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y - 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, texIndex);
        buffer.emplace_back(x + 0.5f, y + 0.5f, z + 0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, texIndex);

        break;
    }

    return buffer;
    //auto vbo = sh::VertexBuffer::Create(&buffer[0].x, buffer.size() * sizeof(Vertex));
}

sh::VertexBufferRef Face::CreateVertexBuffer()
{
    auto vbo = sh::VertexBuffer::Create(0);

    vbo->AddElement(sh::BufferElement(sh::ShaderDataType::Float3, "aPos"));
    vbo->AddElement(sh::BufferElement(sh::ShaderDataType::Float3, "aNormal", true));
    vbo->AddElement(sh::BufferElement(sh::ShaderDataType::Float2, "aUV", true));
    vbo->AddElement(sh::BufferElement(sh::ShaderDataType::Float, "aTexIndex"));

    return vbo;
}

sh::IndexBufferRef Face::CreateIndexBuffer(unsigned faceCount)
{
    const auto elementCount = faceCount * 6u;
    //unsigned* ebo = new unsigned[elementCount];
    std::vector<unsigned> ebo;
    unsigned offset = 0;
    for (unsigned i = 0; i < elementCount; i += 6)
    {
        std::vector<unsigned> indices{ 3 + offset,0 + offset,1 + offset,3 + offset,1 + offset,2 + offset };
        ebo.insert(ebo.end(), indices.begin(), indices.end());
        //memcpy(&ebo[i], indices, 6 * sizeof(unsigned));
        offset += 4;
    }

    return sh::IndexBuffer::Create(ebo.data(),ebo.size());
}

Face::Vertices Cube::CreateVertices(float x, float y, float z, unsigned texIndex)
{
    Face::Vertices ret;

    for (unsigned i = 0; i < 6; i++)
    {
        auto vertices = Face::CreateVertices(BlockFace(i), x, y, z, texIndex);
        ret.insert(ret.end(), vertices.begin(), vertices.end());
    }

    return ret;
}

sh::VertexBufferRef Cube::CreateVertexBuffer()
{
    return Face::CreateVertexBuffer();
}

sh::IndexBufferRef Cube::CreateIndexBuffer()
{
    return Face::CreateIndexBuffer(6);
}
