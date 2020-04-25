//#pragma once
//#include <unordered_map>
//#include <glm/gtx/hash.hpp>
//#include <glm/vec3.hpp>
//
//#include "common/Chunk.hpp"
//#include "client/gl/ChunkRenderer.hpp"
//
//class ChunkRenderer;
//
///**
// * Chunk strategy is a class that requests chunks to be added
// * based on a set of conditions
// */
//class ChunkStrategy
//{
//public:
//    struct ChunkMapValue : public NonCopyable
//    {
//        ChunkMapValue(Chunk&& chunk) noexcept
//            : chunk(std::move(chunk))
//        {
//        }
//
//        Chunk chunk;
//        ChunkMesh mesh;
//    };
//    using ChunkMap = std::unordered_map<glm::ivec3, ChunkMapValue>;
//
//    // Radius in units (multiply by chunkdimension)
//    inline void SetRadius(float radius) noexcept { m_radius = radius; m_chunks.reserve(static_cast<unsigned>(radius * radius * radius + 1u) / chunkSize); }
//    void SetPos(const glm::vec3& pos) noexcept;
//
//    void Update(ChunkContainer& container);
//private:
//
//    float m_radius;
//    glm::vec3 m_pos;
//
//    ChunkMap m_chunks;
//    std::list<std::reference_wrapper<ChunkMapValue>> m_chunksNotFilled;
//};