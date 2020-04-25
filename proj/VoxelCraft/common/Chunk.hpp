#pragma once
#include "BlockData.hpp"

#include <functional>
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <unordered_map>

constexpr glm::uvec3 chunkDimension { 16,16, 16};
constexpr auto chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

/**
 * A chunk contains the data of a certain place in the world
 */
class Chunk
{
public:
    struct Neighbors
    {
        Neighbors()
            : count(0)
        {
            for (int i = 0; i < 6; i++) neighbor.m[i] = nullptr;
        }
        
        int count;
        
        /**
         * The data is ordered like this because
         * when we remove a neighbor such as one on top, we want
         * to remove the neighbor at the bottom, so we can just
         * add 3 and module by the size.
         * This way, I won't have to write if (up remove bottom and up) but
         * I can just use a loop
         *
         * In practice tho, I don't think it really saves a lot.
         */
        union
        {
            Chunk* m[6];
            struct
            {
                Chunk* top;
                Chunk* left;
                Chunk* front;
                Chunk* bottom;
                Chunk* right;
                Chunk* back;
            };
        }neighbor;
    };
    
    //using BlockArray = std::array<std::array<std::array<BlockType, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;
    using BlockArray = BlockType[chunkDimension.x][chunkDimension.y][chunkDimension.z];

    Chunk(const glm::ivec3& pos);

    /**
     * Sets the block type to the correct type
     * based on the position
     */
    void Generate() noexcept; 

    inline glm::ivec3 GetPos() const noexcept { return m_pos; }
    
    inline const BlockArray& GetBlockArray() const noexcept { return m_blocks; }

    // Used for optimization by the renderer
    bool m_modified;
    bool m_isAir;
    Neighbors m_neighbors;

private:
    const glm::ivec3 m_pos;
    
    BlockArray m_blocks;
};

class ChunkManager
{
public:
    using ChunkMap = std::unordered_map<glm::ivec3, Chunk>;
    using ChunkCB = std::function<void(Chunk&)>;
    
    void AddChunk(glm::ivec3 pos);
    // True if chunk existed
    void RemoveChunk(glm::ivec3 pos);

    void Update();
    const std::vector<std::reference_wrapper<Chunk>>& GetModifiedChunks() const;
    ChunkMap& GetChunks();


    ChunkCB m_addCB;
    ChunkCB m_removeCB;
private:
    void GenerateChunk(Chunk& chunk);

    std::vector<std::reference_wrapper<Chunk>> m_modifiedChunks;
    ChunkMap m_chunks;
};