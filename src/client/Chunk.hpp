#pragma once

constexpr glm::uvec3 chunkDimension { 16,16,16 };
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
        Modified,
        Done,
    };

    //using BlockArray = std::array<std::array<std::array<BlockType, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;
    using BlockArray = BlockType[chunkDimension.x][chunkDimension.y][chunkDimension.z];

    explicit Chunk(const BlockDataFactory& factory, glm::ivec3 pos = glm::ivec3(-1));

    /**
     * Sets the block type to the correct type
     * based on the position
     */
    void Generate() noexcept; 

    void SetPos(const glm::ivec3& pos) noexcept;
    glm::ivec3 GetPos() const noexcept;
    
    /**
     * Mostly called at the end of a frame
     * Means that the chunk is done loading/modifying etc
     */
    void MarkDone();
    State GetState() const noexcept;
    const BlockArray& GetBlockArray() const;

private:
    glm::ivec3 m_pos{ -1 };
    
    const BlockDataFactory& m_factory;
    BlockArray m_blocks;
    State m_state = State::New;
};

//class ChunkManager : public Renderable
//{
//public:
//    ChunkManager(const BlockDataFactory& factory, unsigned chunkRadius);
//
//    void SetLoadPos(glm::ivec3 pos) noexcept;
//
//    // Checks if new chunks have to be loaded and old ones have to be unloaded
//    void Update() noexcept;
//
//    virtual const VBO& GetDrawData() const noexcept override final;
//
//private:
//    void UpdateRenderData();
//
//    glm::ivec3 m_pos; 
//    VBO m_buffer;
//
//    const BlockDataFactory& m_factory;
//
//    // x y 
//    std::vector<std::vector<Chunk>> m_chunks;
//};