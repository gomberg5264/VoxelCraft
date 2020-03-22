#pragma once

constexpr glm::uvec3 chunkDimension { 16,256,16 };
constexpr unsigned chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

class Chunk : public Renderable
{
public:
    using BlockArray = std::array<std::array<std::array<BlockType, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;

    explicit Chunk(const BlockDataFactory& factory, glm::ivec3 pos = glm::ivec3(-1));

    void Generate() noexcept;
    void UpdateChunkRenderData();

    void SetPos(const glm::ivec3& pos) noexcept;
    glm::ivec3 GetPos() const noexcept;

    virtual const VBO& GetDrawData() const noexcept override final;

    // If this is true, chunkManager will rebuild draw data
    bool m_modified = false;

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