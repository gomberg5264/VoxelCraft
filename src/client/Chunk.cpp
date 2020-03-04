#include "vcpch.hpp"

Chunk::Chunk(glm::fvec3 pos) noexcept
    : m_pos(pos)
{
}

static int i = 0;
void Chunk::Generate(BlockDataFactory& meta, TextureAtlas& atlas) noexcept
{
    // Generate chunk
    const auto& data = i++ % 2 == 1 ? meta.GetBlockData(BlockType::Grass) : meta.GetBlockData(BlockType::Stone);

    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                glm::fvec3 pos{ x,y,z };

                m_blocks[x][y][z].Init(
                    m_pos + pos,
                    data,
                    atlas);
            }
        }
    }

    UpdateChunkRenderData();
}

void Chunk::UpdateChunkRenderData()
{
    unsigned posI = 0;
    unsigned texI = 0;
    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                const auto& block = m_blocks[x][y][z];
                const auto& posBuf = block.GetPosData();
                const auto& texBuf = block.GetTextureData();

                memcpy(&m_posData.data()[posI], posBuf.buffer, posBuf.size * sizeof(GLfloat));
                memcpy(&m_texData.data()[texI], texBuf.buffer, texBuf.size * sizeof(GLfloat));
            
                posI += posBuf.size;
                texI += texBuf.size;
            }
        }
    }
}

void Chunk::SetPos(const glm::fvec3& pos) noexcept
{
    m_pos = pos - glm::fvec3(chunkDimension);
}

glm::fvec3 Chunk::GetPos() const noexcept
{
    return m_pos;
}

const Renderable::BufferData Chunk::GetPosData() const noexcept
{
    return { m_posData.data(),m_posData.size() };
}

const Renderable::BufferData Chunk::GetTextureData() const noexcept
{
    return { m_texData.data(),m_texData.size() };
}
