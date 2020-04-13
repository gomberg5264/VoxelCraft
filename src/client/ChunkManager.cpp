#include "vcpch.hpp"
#include "ChunkManager.hpp"

void ChunkManager::LoadChunk(ChunkManager::ChunkMapValue& value)
{
    value.chunk.Generate();
    value.chunk.MarkModify();
    value.mesh.Generate(value.chunk);

    m_renderer.RegisterEBOToVAO(value.mesh.m_vao);
}

void ChunkManager::SetPos(const glm::vec3& pos) noexcept
{
    m_pos = pos;

    // Load the center chunk
    // Check for chunk in center
    glm::ivec3 center = glm::ivec3(m_pos);
    center.x -= center.x % chunkDimension.x;
    center.y -= center.y % chunkDimension.y;
    center.z -= center.z % chunkDimension.z;

    //auto& center = m_chunks.at(pos);
    if (m_chunks.count(center) == 0)
    {
        m_chunks.emplace(center ,center);
        LoadChunk(m_chunks.at(center));
        m_chunksNotFilled.push_back(m_chunks.at(center));
    }
}

void ChunkManager::Update()
{
    // Remove out of bounds chunks
    // ---
    {
        std::vector<glm::ivec3> toRemove;
        for (auto& val : m_chunks)
        {
            if (val.second.chunk.m_neighbors.count != 6)
            {
                const float distance = glm::distance(glm::vec3(val.second.chunk.GetPos()), m_pos);
                if (distance > m_radius)
                {
                    // Remove self from other chunks
                    for (int i = 0; i < 6; i++)
                    {
                        auto* dir = val.second.chunk.m_neighbors.neighbor.m[i];
                        if (dir)
                        {
                            val.second.chunk.m_neighbors.neighbor.m[i] = nullptr;
                            val.second.chunk.m_neighbors.count--;

                            dir->m_neighbors.neighbor.m[(i + 3) % 6] = nullptr;
                            dir->m_neighbors.count--;
                            dir->MarkModify(); // This is so the chunk regenerates their mesh
                        }
                    }

                    toRemove.push_back(val.first);
                }
            }
        }

        for (const auto& val : toRemove)
        {
            assert(m_chunks.at(val).chunk.m_neighbors.count == 0 && "Count should be 0. Chunk is not unregistered");

            m_chunksNotFilled.remove_if([&val](const std::reference_wrapper<ChunkMapValue>& value) {return value.get().chunk.GetPos() == val; });
            m_chunks.erase(val);
        }
    }
    
    // Add chunks in range
    // ---
    {
        glm::ivec3 pos = glm::ivec3(m_pos);
        pos.x -= pos.x % chunkDimension.x;
        pos.y -= pos.y % chunkDimension.y;
        pos.z -= pos.z % chunkDimension.z;

        // We fake mt loading by limiting the amount of chunks to load a frame
        int chunkGenLeft = 6;

        // TODO: Do targeted loading
        // Right now, this loads in just a circular pattern
        // It should optimize based on certain conditions that are yet to be specified
        std::function<void()> loadFn = [&]()
        {
            //// If the boi is fully loaded, go to the neighbor
            //if (current.chunk.m_neighbors.count == 6)
            //{
            //    for (int i = 0; i < 6; i++)
            //    {
            //        loadFn(m_chunks.at(current.chunk.m_neighbors.neighbor.m[i]->GetPos()));
            //    }
            //}
            //else
            
            // For the chunks withough fully populated neighbor check if neighbors can be loaded
            for (auto& chunk : m_chunksNotFilled)
            {
                auto& current = chunk.get();
                for (int i = 0; i < 6; i++)
                {
                    if (chunkGenLeft < 0) return;

                    auto* dir = current.chunk.m_neighbors.neighbor.m[i];
                    if (!dir)
                    {
                        //Chunk* top;
                        //Chunk* left;
                        //Chunk* front;
                        //Chunk* bottom;
                        //Chunk* right;
                        //Chunk* back;
                        glm::ivec3 offset(0);
                        if (i == 0) offset.y = (int)chunkDimension.y;
                        else if (i == 1) offset.x = -(int)chunkDimension.x;
                        else if (i == 2) offset.z = (int)chunkDimension.z;
                        else if (i == 3) offset.y = -(int)chunkDimension.y;
                        else if (i == 4) offset.x = (int)chunkDimension.x;
                        else if (i == 5) offset.z = -(int)chunkDimension.z;
                        glm::ivec3 neighborPos = current.chunk.GetPos() + offset;

                        const float distance = glm::distance(glm::vec3(neighborPos), m_pos);

                        // If in radius but not yet loaded
                        if (distance < m_radius)
                        {
                            // Add the chunk
                            // Create and put in list
                            m_chunks.emplace(neighborPos, neighborPos);
                            auto& neighbor = m_chunks.at(neighborPos);
                            LoadChunk(neighbor);

                            // Set up neighbors
                            //dir = &neighbor.chunk;
                            current.chunk.m_neighbors.neighbor.m[i] = &neighbor.chunk;
                            current.chunk.m_neighbors.count++;

                            neighbor.chunk.m_neighbors.neighbor.m[(i + 3) % 6] = &current.chunk;
                            neighbor.chunk.m_neighbors.count++;

                            chunkGenLeft--;
                            m_chunksNotFilled.push_back(neighbor);
                        }
                    }
                }
            }
        };
        loadFn();

        // Remove chunks from edge list if their neighbors are full
        // Also regenreate them since we can cull based on adjacent faces
        m_chunksNotFilled.remove_if([](std::reference_wrapper<ChunkMapValue>& value) 
            {
                if (value.get().chunk.m_neighbors.count == 6)
                {
                    value.get().chunk.MarkModify();
                    return true; 
                }
                return false;
            });

        static int frameCount = 0;
        frameCount++;
        if (frameCount > 10)
        {
            std::cout << m_chunks.size() << " Chunks" << std::endl;
            frameCount = 0;
        }
    }
}

void ChunkManager::Render()
{
    // TODO: Do culling
    for (auto& chunk : m_chunks)
    {
        if (!chunk.second.chunk.m_isAir)
        {
            m_renderer.Render(chunk.second.mesh);
            if (chunk.second.chunk.GetState() == Chunk::State::Modify)
            {
                chunk.second.mesh.Generate(chunk.second.chunk);
                chunk.second.chunk.MarkDone();
            }
        }
    }
}
