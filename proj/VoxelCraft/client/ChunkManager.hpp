#pragma once
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include "common/Chunk.hpp"
#include "client/gl/ChunkRenderer.hpp"

class ChunkRenderer;

/**
 * A ChunkManager loads multiple chunks in memory based on conditions
 * There can be multiple types of chunk managers
 * One for the player, this chunk manager would load chunks in a circle
 * One for the server, it would get a collection of chunks that it will keep track of
 *
 * TODO You probably want to abstract this since it is different for the client and server
 */
class ChunkManager
{
public:
    struct ChunkMapValue : public NonCopyable
    {
        ChunkMapValue(Chunk&& chunk) noexcept
            : chunk(std::move(chunk))
        {
        }

        Chunk chunk;
        ChunkMesh mesh;
    };
    using ChunkMap = std::unordered_map<glm::ivec3, ChunkMapValue>;

    /**
     * Reference to chunk renderer is needed to register buffers
     * for new chunks
     */
    ChunkManager(ChunkRenderer& renderer) noexcept
        : m_radius(0)
        , m_pos(0)
        , m_renderer(renderer) {}

    // Radius in units (multiply by chunkdimension)
    inline void SetRadius(float radius) noexcept { m_radius = radius; m_chunks.reserve(static_cast<unsigned>(radius * radius * radius + 1u) / chunkSize); }
    void SetPos(const glm::vec3& pos) noexcept;

    void Update();
    
    /**
     * Pushes render requests to the renderer given in the constructor
     * TODO: We may want to remove this code from the chunkManager and instead
     * move all that logic into ChunkRenderer. We would have to expose the chunk map in that case.
     * It comes down to the question whether the renderer should be concerned with doing culling or if 
     * the chunk manager should do the culling
     */
    void Render();

private:

    void LoadChunk(ChunkMapValue& value);

    float m_radius;
    glm::vec3 m_pos;

    ChunkMap m_chunks;
    std::list<std::reference_wrapper<ChunkMapValue>> m_chunksNotFilled;
    ChunkRenderer& m_renderer;
};