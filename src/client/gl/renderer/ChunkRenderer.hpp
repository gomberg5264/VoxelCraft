#pragma once

class ChunkMesh
{
public:
    ChunkMesh(unsigned index);
    void Generate(const Chunk& chunk);

    const unsigned m_index;
    Primitive::Face::Buffer m_buffer;
};

/**
 * Specialized renderer for chunks
 */
class ChunkRenderer
{
public:
    ChunkRenderer(unsigned chunkCount);
    ~ChunkRenderer();

    void SetVP(const glm::mat4& vp);
    void Render(const ChunkMesh& mesh, bool updateDrawData);
    void Display();

private:
    std::vector<std::reference_wrapper<const ChunkMesh>> m_updateQueue;
    
    Shader m_shader;
    const unsigned m_chunkCount;
    unsigned m_drawCount;

    struct Buffer
    {
        unsigned vertices;
        unsigned indices;
        unsigned vao;
    }m_buffer;
};