#pragma once
#include "Common/BlockData.h"

#include <Shinobu/Renderer/VertexArray.h>
#include <Shinobu/Renderer/VertexBuffer.h>

namespace Face
{
    struct Vertex
    {
        Vertex(
            float x, float y, float z,
            float nx, float ny, float nz,
            float u, float v, float tIndex)
            : x(x), y(y), z(z)
            , nx(nx), ny(ny), nz(nz)
            , u(u), v(v),
            tIndex(tIndex)
        {}

        float x, y, z;
        float nx, ny, nz;
        float u, v;
        float tIndex;
    };
    using Vertices = std::vector<Vertex>;
    
    Vertices CreateVertices(BlockFace dir, float x, float y, float z, unsigned texIndex);
    sh::VertexBufferRef CreateVertexBuffer();
    sh::IndexBufferRef CreateIndexBuffer(unsigned faceCount);
}

namespace Cube
{
    Face::Vertices CreateVertices(float x, float y, float z, unsigned texIndex);
    sh::VertexBufferRef CreateVertexBuffer();
    sh::IndexBufferRef CreateIndexBuffer();
}