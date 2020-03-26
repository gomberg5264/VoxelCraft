#pragma once
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include "client/Chunk.hpp"
#include "client/gl/renderer/ChunkRenderer.hpp"

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
    struct ChunkMapValue
    {
        ChunkMapValue(Chunk&& chunk) noexcept
            : chunk(std::move(chunk))
        {
            mesh.Generate(chunk);
        }

        Chunk chunk;
        ChunkMesh mesh;
    };
    using ChunkMap= std::unordered_map<glm::ivec3, ChunkMapValue>;

    /** 
     * Reference to chunk renderer is needed to register buffers 
     * for new chunks
     */
    ChunkManager(ChunkRenderer& renderer) noexcept
        : m_radius(0)
        , m_pos(0)
        , m_renderer(renderer) {}
    
    inline void SetRadius(float radius) noexcept { m_radius = radius; }
    inline void SetPos(const glm::vec3& pos) noexcept { m_pos = pos; }

    void Update();
    void Render() const;

private:
    float m_radius;
    glm::vec3 m_pos;

    ChunkMap m_chunks;
    ChunkRenderer& m_renderer;
};