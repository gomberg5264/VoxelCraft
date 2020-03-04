#pragma once

constexpr glm::uvec3 chunkDimension { 16,256,16 };
constexpr unsigned chunkSize{ chunkDimension.x * chunkDimension.y * chunkDimension.z };

class Chunk : public Renderable
{
public:
    explicit Chunk(glm::fvec3 pos = glm::fvec3(0.f)) noexcept;

    void Generate(BlockDataFactory& meta, TextureAtlas& atlas) noexcept;
    void UpdateChunkRenderData();

    void SetPos(const glm::fvec3& centerPos) noexcept;
    glm::fvec3 GetPos() const noexcept;

    virtual const BufferData GetPosData() const noexcept override final;
    virtual const BufferData GetTextureData() const noexcept override final;
    virtual const unsigned GetDrawCount() const noexcept override final { return chunkSize; }

private:
    glm::fvec3 m_pos;
    std::array<GLfloat, chunkSize * 3> m_posData;
    std::array<GLfloat, chunkSize * 6 * 2> m_texData;

    using BlockArray = std::array<std::array<std::array<Block, chunkDimension.z>, chunkDimension.y>, chunkDimension.x>;

    BlockArray m_blocks;
};