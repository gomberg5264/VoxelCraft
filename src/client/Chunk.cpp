#include "vcpch.hpp"

Chunk::Chunk(glm::fvec3 pos) noexcept
    : m_pos(pos)
{
}

void Chunk::Generate(BlockDataFactory& meta, TextureAtlas& atlas) noexcept
{
    static int i = 0;
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
    m_posData.clear();
    m_texData.clear();

    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                const auto& block = m_blocks[x][y][z];
                const auto& posBuf = block.GetPosData();
                const auto& texBuf = block.GetTextureData();

                m_posData.insert(std::begin(m_posData),std::begin(posBuf),std::end(posBuf));
                m_texData.insert(std::begin(m_texData),std::begin(texBuf),std::end(texBuf));
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

const std::vector<GLfloat>& Chunk::GetPosData() const noexcept
{
    return m_posData;
}

const std::vector<GLfloat>& Chunk::GetTextureData() const noexcept
{
    return m_texData;
}
