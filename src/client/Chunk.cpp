#include "vcpch.hpp"

Chunk::Chunk(glm::fvec3 pos) noexcept
    : m_pos(pos)
    , m_posBuf(chunkSize)
    , m_texBuf(chunkSize)
    , m_drawCount(0)
{
}

static int i = 0;
void Chunk::Generate(BlockDataFactory& meta, TextureAtlas& atlas) noexcept
{
    // Generate chunk
    // The lower the more flat
    constexpr float flatFactor = 0.10f;

    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                // Execute generation criteria
                const float perlin = glm::perlin(glm::fvec2(float(x + m_pos.x) / chunkDimension.x, float(z + m_pos.z)/chunkDimension.z) * flatFactor);
                // perlin [0,1]
                const int height = ((perlin + 1.f) / 2.f) * (chunkDimension.y * 0.5f) + (chunkDimension.y * 0.5f);
                glm::fvec3 pos{ x,y,z };
                BlockData data;

                if (y > height)
                {
                    data = meta.GetBlockData(BlockType::Air);
                }
                else
                {
                    if (y == height)
                    {
                        data = meta.GetBlockData(BlockType::Grass);
                    }
                    else if (y > (height - 3))
                    {
                        data = meta.GetBlockData(BlockType::Dirt);
                    }
                    else
                    {
                        data = meta.GetBlockData(BlockType::Stone);
                    }
                }

                m_blocks[x][y][z].Init(
                    m_pos + pos,
                    data,
                    atlas);

            }
        }
    }

    UpdateChunkRenderData();
}

bool IsVisible(int x, int y, int z, const Chunk::BlockArray& blocks)
{
    // Block is only culled if it is surrounded on all sides

    // If it is on the edge of a chunk, we will just render it
    if (x < 1 || x >= chunkDimension.x - 1 ||
        y < 1 || y >= chunkDimension.y - 1 ||
        z < 1 || z >= chunkDimension.z - 1) return true;

    // If visible from any side we should just render
    if (!blocks[x + 1][y][z].GetData().isSolid) return true;
    if (!blocks[x - 1][y][z].GetData().isSolid) return true;
    if (!blocks[x][y + 1][z].GetData().isSolid) return true;
    if (!blocks[x][y - 1][z].GetData().isSolid) return true;
    if (!blocks[x][y][z + 1].GetData().isSolid) return true;
    if (!blocks[x][y][z - 1].GetData().isSolid) return true;

    // This means fully encapsulated
    return false;
}

void Chunk::UpdateChunkRenderData()
{
    unsigned posI = 0;
    unsigned texI = 0;
    m_drawCount = 0;
    for (unsigned x = 0; x < chunkDimension.x; x++)
        for (unsigned y = 0; y < chunkDimension.y; y++)
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                const auto& block = m_blocks[x][y][z];
                if (block.GetData().isSolid && IsVisible(x,y,z,m_blocks))
                {
                    const auto& posBuf = block.GetPosData();
                    const auto& texBuf = block.GetTextureData();

                    m_posBuf.CopyFrom(posBuf, posI);
                    m_texBuf.CopyFrom(texBuf, texI);
            
                    m_drawCount++;
                    posI += posBuf.size;
                    texI += texBuf.size;
                }
            }
}

void Chunk::SetPos(const glm::fvec3& pos) noexcept
{
    m_pos = pos;
}

glm::fvec3 Chunk::GetPos() const noexcept
{
    return m_pos;
}

const Buffer& Chunk::GetPosData() const noexcept
{
    return m_posBuf;
}

const Buffer& Chunk::GetTextureData() const noexcept
{
    return m_texBuf;
}

const unsigned Chunk::GetDrawCount() const noexcept
{
    return m_drawCount;
}
