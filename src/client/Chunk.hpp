#pragma once

constexpr glm::uvec3 chunkDimension { 16,256,16 };
constexpr unsigned long chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

class Chunk : public Renderable
{
public:
    enum class State
    {
        New,
        Modified,
        Done,
    };

    using BlockArray = std::array<std::array<std::array<BlockType, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;

    explicit Chunk(const BlockDataFactory& factory, glm::ivec3 pos = glm::ivec3(-1));

    /**
     * Sets the block type to the correct type
     */
    void Generate() noexcept; 

    /**
     * Caches a new buffer using the current pos
     * Make sure to call generate and set a pos beforehand
     */
    void UpdateChunkRenderData();

    void SetPos(const glm::ivec3& pos) noexcept;
    glm::ivec3 GetPos() const noexcept;

    virtual const VBO& GetDrawData() const noexcept override final;

    // If this is true, chunkManager will rebuild draw data
    State m_state = State::New;

private:
    // Chunk manager updates if chunk pos differs.
    // This means that initial chunks need to have a pos that
    // is not a multiple of 16
    glm::ivec3 m_pos{ -1 };
    VBO m_buffer;
    
    const BlockDataFactory& m_factory;
    BlockArray m_blocks;
};

class ChunkManager : public Renderable
{
public:
    ChunkManager(const BlockDataFactory& factory, unsigned chunkRadius);

    void SetLoadPos(glm::ivec3 pos) noexcept;

    // Checks if new chunks have to be loaded and old ones have to be unloaded
    void Update() noexcept;

    virtual const VBO& GetDrawData() const noexcept override final;

private:
    void UpdateRenderData();

    glm::ivec3 m_pos; 
    VBO m_buffer;

    const BlockDataFactory& m_factory;

    // x y 
    std::vector<std::vector<Chunk>> m_chunks;
};