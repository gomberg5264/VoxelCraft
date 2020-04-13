#pragma once
#include "BlockData.hpp"

constexpr glm::uvec3 chunkDimension { 16,16, 16};
constexpr auto chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

/**
 * A chunk contains the data of a certain place in the world
 */
class Chunk
{
public:
    enum class State
    {
        New,
        Modify,
        Done,
    };

    struct Neighbors
    {
        Neighbors()
            : count(0)
        {
            for (int i = 0; i < 6; i++) neighbor.m[i] = nullptr;
        }
        
        int count;
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
    
    inline void MarkModify() noexcept { m_state = State::Modify; }
    inline void MarkDone() noexcept { m_state = State::Done; }
    inline State GetState() const noexcept { return m_state; }

    inline const BlockArray& GetBlockArray() const noexcept { return m_blocks; }

    // Used for optimization by the renderer
    bool m_isAir;
    Neighbors m_neighbors;

private:
    const glm::ivec3 m_pos;
    
    BlockArray m_blocks;
    State m_state = State::New;
};