#pragma once

constexpr glm::uvec3 chunkDimension { 16,16,16};
constexpr unsigned long chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

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

private:
    const glm::ivec3 m_pos;
    
    BlockArray m_blocks;
    State m_state = State::New;
};

/**
 * ChunkManager loads multiple chunks in memory based on conditions
 * There can be multiple types of chunk managers
 * One for the player, this chunk manager would load chunks in a circle
 * One for the server, it would get a collection of chunks that it will keep track of
 */
class PlayerChunkManager
{

};