#pragma once
#include "Common/BlockData.h"

#include <Shinobu/Renderer/VertexArray.h>
#include <Shinobu/Renderer/VertexBuffer.h>

namespace Face
{
    sh::VertexBufferRef CreateVertexBuffer(BlockFace dir, float x, float y, float z, unsigned texIndex);
    sh::IndexBufferRef CreateIndexBuffer(unsigned faceCount);
}