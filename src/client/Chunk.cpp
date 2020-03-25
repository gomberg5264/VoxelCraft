#include "vcpch.hpp"

Chunk::Chunk(glm::ivec3 pos)
    : m_pos(pos)
{
}

void Chunk::Generate() noexcept
{
    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                auto DefaultBlockGen = [](float x, float y, float z) -> BlockType
                {
                    // The lower the more flat
                    constexpr float flatFactor = 0.10f;

                    // Execute generation criteria
                    const float perlin = glm::perlin(
                        glm::fvec2(x,z) * flatFactor);

                    assert(perlin >= -1.f && "Why is perlin negative???");
                    
                    // perlin [0,1]
                    const unsigned height = static_cast<unsigned>(
                        (perlin + 1.f) / 2.f) * (chunkDimension.y * 0.5f) + (chunkDimension.y * 0.5f);
                    glm::fvec3 pos{ x,y,z };

                    BlockType type;
                    if (y > height)
                    {
                        type = (BlockType::Air);
                    }
                    else
                    {
                        if (y == height)
                        {
                            type = (BlockType::Grass);
                        }
                        else if (y > (height - 3))
                        {
                            type = (BlockType::Dirt);
                        }
                        else
                        {
                            type = (BlockType::Stone);
                        }
                    }
                    return type;
                };
                auto DebugBlockGen = [](float x, float y, float z)->BlockType
                {
                    //return BlockType::Grass;
                    if (y + 1 == chunkDimension.y - 2)
                        return BlockType::Air;
                    else if (y + 1 == chunkDimension.y)
                        return BlockType::Grass; 
                    else if (y + 1 > chunkDimension.y - 4)
                        return BlockType::Dirt;
                    else
                        return BlockType::Stone;
                };

                glm::fvec3 blockPos(x, y, z);
                blockPos += m_pos;

                //m_blocks[x][y][z] = DefaultBlockGen(blockPos.x,blockPos.y,blockPos.z);
                m_blocks[x][y][z] = DebugBlockGen(blockPos.x, blockPos.y, blockPos.z);
            }
        }
    }
}

/*
bool IsVisible(int x, int y, int z, const Chunk::BlockArray& blocks, const BlockDataFactory& factory)
{
    // Block is only culled if it is surrounded on all sides

    // If it is on the edge of a chunk, we will just render it
    if (x < 1 || x >= static_cast<int>(chunkDimension.x) - 1 ||
        y < 1 || y >= static_cast<int>(chunkDimension.y) - 1 ||
        z < 1 || z >= static_cast<int>(chunkDimension.z) - 1) return true;

    // If visible from any side we should just render
    if (!factory.GetBlockData(blocks[x + 1][y][z]).isSolid) return true;
    if (!factory.GetBlockData(blocks[x - 1][y][z]).isSolid) return true;
    if (!factory.GetBlockData(blocks[x][y + 1][z]).isSolid) return true;
    if (!factory.GetBlockData(blocks[x][y - 1][z]).isSolid) return true;
    if (!factory.GetBlockData(blocks[x][y][z + 1]).isSolid) return true;
    if (!factory.GetBlockData(blocks[x][y][z - 1]).isSolid) return true;

    // This means fully encapsulated
    return false;
}
*/

void Chunk::SetPos(const glm::ivec3& pos) noexcept
{
    m_pos = pos;
}

glm::ivec3 Chunk::GetPos() const noexcept
{
    return m_pos;
}

void Chunk::MarkDone()
{
    m_state = State::Done;
}

Chunk::State Chunk::GetState() const noexcept
{
    return m_state;
}

const Chunk::BlockArray& Chunk::GetBlockArray() const
{
    return m_blocks;
}