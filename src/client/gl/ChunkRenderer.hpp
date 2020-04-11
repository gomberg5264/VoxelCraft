#pragma once
#include "client/gl/Primitive.hpp"
#include "client/gl/Buffer.hpp"
#include "client/gl/Shader.hpp"

/**
 * Fills the VAO based on the Chunk data. 
 */
class ChunkMesh
{
public:
    ChunkMesh();
    void Generate(const Chunk& chunk);

    VAO m_vao;
    Primitive::Face::Buffer m_buffer;
};

/**
 * The chunk renderer has the index buffers for chunks since this is constant
 * It also has the shader
 * TODO perhaps refactor this into the chunk itself since the renderer seems redundant
 * Right now, the chunk renderer is a class that queues render calls.
 * By batching the calls, we only have to set the shader once. 
 * The chunkRenderer also contains a ebo that is the max size. We can make this static however.
 * So simply said, the chunkrenderer makes it so that we set the shader once. 
 */
class ChunkRenderer
{
public:
    ChunkRenderer();
    ~ChunkRenderer();

    void RegisterVAO(VAO& vao);

    void SetVP(const glm::mat4& vp) noexcept;
    void SetLightDirection(const glm::vec3& lightDir) noexcept;

    /**
     * @param color [0,1]
     */
    void SetAmbientLight(const glm::vec3& color) noexcept;

    void Render(const ChunkMesh& mesh, bool updateDrawData);
    void Display() noexcept;

private:
    struct Command
    {
        const ChunkMesh& chunk;
        bool upload;
    };
    std::vector<Command> m_renderQueue;

    Shader m_shader;

    const unsigned m_elementCount;
    unsigned m_ebo;
};