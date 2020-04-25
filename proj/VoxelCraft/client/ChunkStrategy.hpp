#pragma once
#include "common/Chunk.hpp"
#include "client/gl/ChunkRenderer.hpp"

#include <functional>

/**
 * Chunk strategy is a class that requests chunks to be added
 * based on a set of conditions
 */
class ChunkStrategy
{
public:
    // Radius in units (multiply by chunkdimension)
    inline void SetRadius(float radius) noexcept { m_radius = radius; }
    void SetPos(const glm::vec3& pos) noexcept;

    void Update(ChunkManager& chunks);

private:
    float m_radius;
    glm::vec3 m_pos;

    std::list<std::reference_wrapper<Chunk>> m_chunksNotFilled;
};