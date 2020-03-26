#include "vcpch.hpp"
#include "Chunk.hpp"

Chunk::Chunk(const glm::ivec3& pos)
    : m_isAir(true)
    , m_pos(pos)
{
    // Pos should be multiple of 16
    assert(pos.x % chunkDimension.x == 0);
    assert(pos.y % chunkDimension.y == 0);
    assert(pos.z % chunkDimension.z == 0);
}

void Chunk::Generate() noexcept
{
    bool isAirOnly = true;
    for (unsigned x = 0; x < chunkDimension.x; x++)
    {
        for (unsigned y = 0; y < chunkDimension.y; y++)
        {
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                auto DefaultBlockGen = [&isAirOnly](int x, int y, int z) -> BlockType
                {
                    // The lower the more flat
                    constexpr float flatFactor = 5.0f;//0.25f;
                    constexpr float xVarianceFactor = 1.f / float(chunkDimension.x) * flatFactor;
                    constexpr float zVarianceFactor = 1.f / float(chunkDimension.z) * flatFactor;

                    // Execute generation criteria
                    const float noise = glm::simplex(
                        glm::fvec2(x * xVarianceFactor,z * zVarianceFactor));

                    assert(noise >= -1.f && "Why is noise negative???");
                    
                    // noise [0,1]
                    const int height = static_cast<int>(
                        (noise + 1.f) / 2.f * (float(chunkDimension.y) * 0.5f) + (float(chunkDimension.y) * 0.5f));
                    glm::fvec3 pos{ x,y,z };

                    //std::cout << x << ' ' << y << ' ' << (perlin + 1.f) / 2.f)<< '\n';

                    BlockType type;
                    if (y > height)
                    {
                        type = (BlockType::Air);
                    }
                    else
                    {
                        isAirOnly = false;

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
                    if (y == 5 && x > 3 && x < 12 && z > 3 && z < 12)
                        return BlockType::Air;

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

                glm::ivec3 blockPos(x, y, z);
                blockPos += m_pos;

                m_blocks[x][y][z] = DefaultBlockGen(blockPos.x,blockPos.y,blockPos.z);
                //m_blocks[x][y][z] = DebugBlockGen(blockPos.x, blockPos.y, blockPos.z);
            }
        }
    }

    m_isAir = isAirOnly;
}