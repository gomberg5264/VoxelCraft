#include "vcpch.hpp"

Chunk::Chunk(const BlockDataFactory& factory, glm::ivec3 pos)
    : m_pos(pos)
    , m_buffer(chunkSize)
    , m_factory(factory)
{
}

void Chunk::Generate() noexcept
{
    // Generate chunk
    // The lower the more flat
    constexpr float flatFactor = 0.10f;

    for (int x = 0; x < chunkDimension.x; x++)
    {
        for (int y = 0; y < chunkDimension.y; y++)
        {
            for (int z = 0; z < chunkDimension.z; z++)
            {
                // Execute generation criteria
                const float perlin = glm::perlin(glm::fvec2(float(x + m_pos.x) / chunkDimension.x, float(z + m_pos.z)/chunkDimension.z) * flatFactor);
                assert(perlin >= 0.f && "Why is perlin negative???");
                // perlin [0,1]
                const unsigned height = ((perlin + 1.f) / 2.f) * (chunkDimension.y * 0.5f) + (chunkDimension.y * 0.5f);
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

                m_blocks[x][y][z] = type;

            }
        }
    }

    UpdateChunkRenderData();
}

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

void Chunk::UpdateChunkRenderData()
{
    for (unsigned x = 0; x < chunkDimension.x; x++)
        for (unsigned y = 0; y < chunkDimension.y; y++)
            for (unsigned z = 0; z < chunkDimension.z; z++)
            {
                const auto& data = m_factory.GetBlockData(m_blocks[x][y][z]);
                if (data.isSolid && IsVisible(x,y,z,m_blocks,m_factory))
                {
                    auto buffer = data.CreateVBO(glm::fvec3{ x,y,z } + glm::fvec3(m_pos));
                    const auto stride = buffer.GetActiveSize() * sizeof(VBO::Vertex);
                    memcpy(
                        &m_buffer[m_buffer.GetActiveSize()],
                        buffer.Data(),
                        stride);
                    m_buffer.SetActiveSize(m_buffer.GetActiveSize() + buffer.GetActiveSize());
                }
            }
}

void Chunk::SetPos(const glm::ivec3& pos) noexcept
{
    m_pos = pos;
}

glm::ivec3 Chunk::GetPos() const noexcept
{
    return m_pos;
}

const VBO& Chunk::GetDrawData() const noexcept
{
    return m_buffer;
}

ChunkManager::ChunkManager(const BlockDataFactory& factory, unsigned chunkRadius)
    : m_buffer(chunkSize* chunkRadius * 2 * chunkRadius * 2)
    , m_factory(factory)
{
    //m_buffer.SetActiveSize(m_buffer.GetSize());
    m_chunks.resize(chunkRadius * 2u);
    for (auto& vertical : m_chunks)
    {
        vertical.reserve(chunkRadius * 2u);
        for (unsigned i = 0; i < chunkRadius * 2; i++)
        {
            //chunk.resize(chunkRadius * 2, Chunk(factory));
            vertical.emplace_back(factory);
        }
    }
}

void ChunkManager::SetLoadPos(glm::ivec3 pos) noexcept
{
    m_pos = pos;
}

void ChunkManager::Update() noexcept
{
    // 1. Calculate chunk pos for each chunk
    // 2. If they differ. add it as a task to regenerate
    // 3. ???
    // 4. win
    // 5. Oh, we also check if a chunk has modified itself

    std::vector<std::reference_wrapper<Chunk>> toGenerate;

    // Assume rhs
    const int radius = m_chunks.size() / 2;
    int z = -radius;
    int x = -radius;
    std::cout << radius << '\n';
    bool rebuildDrawBuffer = false;
    
    // Calculate pos on a chunk basis
    // We divide and multiple so that the pos is a multiple of chunkDimension
    // I think volatile makes the compiler not optimize it away
    volatile glm::ivec3 pos = m_pos / glm::ivec3(chunkDimension) * glm::ivec3(chunkDimension);

    // Left top -> bottom right
    for (auto& vertical : m_chunks)
    {
        for (auto& chunk : vertical)
        {
            // Calculate chunk pos
            glm::ivec3 newPos{
                pos.x + x * static_cast<int>(chunkDimension.x),
                0,
                pos.z + z * static_cast<int>(chunkDimension.z)};

            if (chunk.GetPos() != newPos)
            {
                chunk.SetPos(newPos);
                toGenerate.push_back(chunk);
            }
            else if(chunk.m_state != Chunk::State::Done)
            {
                // Beautifull oop right here
                // love changing state in some random class
                chunk.m_state = Chunk::State::Done;
                rebuildDrawBuffer = true;
            }
            x++;
        }
        z++;
        x = -radius;
    }
    // TODO This should be MT
    for (auto& chunk : toGenerate)
        chunk.get().Generate();

    // If any change has happened to the chunk, rebuild the draw data since we cache it
    if (!toGenerate.empty() || rebuildDrawBuffer) UpdateRenderData();
}

const VBO& ChunkManager::GetDrawData() const noexcept
{
    return m_buffer;
}

void ChunkManager::UpdateRenderData()
{
    //unsigned offset = 0;
    m_buffer.SetActiveSize(0);
    for (const auto& vertical : m_chunks)
        for (const auto& chunk : vertical)
        {
            const auto& dData = chunk.GetDrawData();
            memcpy(
                &m_buffer.Data()[m_buffer.GetActiveSize()], 
                dData.Data(), 
                dData.GetActiveSize() * sizeof(VBO::Vertex));
            m_buffer.SetActiveSize(m_buffer.GetActiveSize() + dData.GetActiveSize());// offset += dData.GetSize();
        }
}
