#pragma once

constexpr glm::uvec3 chunkDimension { 16,256,16 };
constexpr unsigned chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

class Chunk : public Renderable
{
public:
    using BlockArray = std::array<std::array<std::array<BlockType, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;

    explicit Chunk(const BlockDataFactory& factory, glm::fvec3 pos = glm::fvec3(0.f));

    void Generate() noexcept;
    void UpdateChunkRenderData();

    glm::fvec3 GetPos() const noexcept;

    virtual const VBO& GetDrawData() const noexcept override final;

private:
    const glm::fvec3 m_pos;
    VBO m_buffer;
    
    const BlockDataFactory& m_factory;
    BlockArray m_blocks;
};