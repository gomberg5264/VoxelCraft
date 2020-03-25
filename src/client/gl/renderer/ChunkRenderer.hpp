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
 * The chunk renderer handles all the rendering for chunks.
 * This is handles here so that we only have to change the state once.
 * Also by doing this, we won't have to recreate a vertex array every time 
 */
class ChunkRenderer
{
public:
    ChunkRenderer();
    ~ChunkRenderer();

    unsigned GenerateIndex();

    void SetVP(const glm::mat4& vp);
    void Render(const ChunkMesh& mesh, bool updateDrawData);
    void Display();

private:
    
    struct Command
    {
        const ChunkMesh& mesh;
        bool upload;
    };
    std::vector<Command> m_renderQueue;

    Shader m_shader;

    unsigned m_elementCount;
    unsigned m_ebo;
    std::vector<VAO> m_vaos;
};