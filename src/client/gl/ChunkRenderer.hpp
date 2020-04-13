#pragma once
#include "client/gl/Buffer.hpp"
#include "client/gl/Primitive.hpp"
#include "client/gl/Shader.hpp"

/**
 * Fills the VAO based on the Chunk data. 
 */
class ChunkMesh
{
public:
    ChunkMesh();

    /**
     * This function generates and uploads the mesh to the gpu
     */
    void Generate(const Chunk& chunk);

    VAO m_vao;
    size_t m_elemCount;
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

    /**
     * Since the VAO uses the EBO inside ChunkRenderer, we make sure that 
     * VAO is bound to that EBO with this function
     */
    void RegisterEBOToVAO(const VAO& vao);

    void SetVP(const glm::mat4& vp) noexcept;
    void SetSkyLightDirection(const glm::vec3& lightDir) noexcept;
    void SetSkyIntensity(float intensity) noexcept;

    /**
     * @param color [0,1]
     */
    void SetSkyLightColor(const glm::vec3& color) noexcept;
    //void SetAmbientLight(const glm::vec3& color) noexcept;
    
    void Render(const ChunkMesh& mesh);
    void Display() noexcept;

private:
    std::vector<std::reference_wrapper<const ChunkMesh>> m_renderQueue;

    Shader m_shader;
    EBO m_ebo;
};