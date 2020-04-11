#include "vcpch.hpp"
#include "common/BlockData.hpp"
#include "common/Chunk.hpp"

#include "client/gl/ChunkRenderer.hpp"

ChunkMesh::ChunkMesh()
{
    m_vao.AddVBO(
        {
            { 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
            { 1, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
            { 2, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 5u * sizeof(float) },
        });
}


bool IsVisible(BlockFace dir, int x, int y, int z, const Chunk::BlockArray& blocks)
{
    // Block is only culled if it is surrounded on all sides

    // If it is on the edge of a chunk, we will just render it
    if ((dir == BlockFace::Left && x < 1) || (dir == BlockFace::Right && x >= static_cast<int>(chunkDimension.x) - 1) ||
        (dir == BlockFace::Bottom && y < 1) || (dir == BlockFace::Top && y >= static_cast<int>(chunkDimension.y) - 1) ||
        (dir == BlockFace::Back && z < 1) || (dir == BlockFace::Front && z >= static_cast<int>(chunkDimension.z) - 1)) return true;

    // If visible from any side we should just render
    if (dir == BlockFace::Right && !BlockDataFactory::GetInstance().GetBlockData(blocks[x + 1][y][z]).isSolid) return true;
    else if (dir == BlockFace::Left && !BlockDataFactory::GetInstance().GetBlockData(blocks[x - 1][y][z]).isSolid) return true;
    else if (dir == BlockFace::Top && !BlockDataFactory::GetInstance().GetBlockData(blocks[x][y + 1][z]).isSolid) return true;
    else if (dir == BlockFace::Bottom && !BlockDataFactory::GetInstance().GetBlockData(blocks[x][y - 1][z]).isSolid) return true;
    else if (dir == BlockFace::Front && !BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z + 1]).isSolid) return true;
    else if (dir == BlockFace::Back && !BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z - 1]).isSolid) return true;

    // This means fully encapsulated
    return false;
}

void ChunkMesh::Generate(const Chunk& chunk)
{
    const auto& blocks = chunk.GetBlockArray();
    for (unsigned y = 0; y < chunkDimension.y; y++)
        for (unsigned z = 0; z < chunkDimension.z; z++)
            for (unsigned x = 0; x < chunkDimension.x; x++)
            {
                const auto& bData = BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z]);
                if (!bData.isSolid)
                {
                    continue;
                }

                glm::fvec3 bPos(x, y, z);
                bPos += chunk.GetPos();

                // Add all 6 directions
                for (int i = 0; i < BlockFace::Count; i++)
                {
                    if (IsVisible(BlockFace(i), x, y, z, blocks))
                    {
                        auto buffer = Primitive::Face::MakeBuffer(
                            BlockFace(i), bPos.x, bPos.y, bPos.z, bData.texture[i]);

                        m_buffer.vertices.insert(
                            m_buffer.vertices.end(),
                            buffer.vertices.begin(),
                            buffer.vertices.end());
                    }
                }
            }
}

ChunkRenderer::ChunkRenderer()
    : m_shader("res/shaders/face.vert","res/shaders/face.frag")
    // 6 indices per face, 6 faces per voxel
    , m_elementCount(6u * 6u * chunkSize)
    , m_ebo(0)
{
    unsigned* ebo = new unsigned[m_elementCount];
    unsigned offset = 0;
    for (unsigned i = 0; i < m_elementCount; i += 6)
    {
        unsigned indices[]{ 3 + offset,0 + offset,1 + offset,3 + offset,1 + offset,2 + offset };
        memcpy(&ebo[i], indices, 6 * sizeof(unsigned));
        offset += 4;
    }

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementCount * sizeof(unsigned), ebo, GL_STATIC_DRAW);

    delete[] ebo;
}

ChunkRenderer::~ChunkRenderer()
{
    glDeleteBuffers(1, &m_ebo);
}

void ChunkRenderer::RegisterVAO(VAO& vao)
{
    vao.Bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    vao.Unbind();
}

void ChunkRenderer::SetVP(const glm::mat4& vp)
{
    m_shader.Use();
    m_shader.SetMatrix("aVP", glm::value_ptr(vp));
}

void ChunkRenderer::Render(const ChunkMesh& mesh, bool updateDrawData)
{
    m_renderQueue.emplace_back(Command{mesh, updateDrawData});
}

void ChunkRenderer::Display()
{
    m_shader.Use();
    m_shader.SetMatrix("aModel", glm::value_ptr(glm::mat4(1)));

    for (const auto& mesh : m_renderQueue)
    {
        auto& vao = mesh.chunk.m_vao;
        
        vao.Bind();

        if (mesh.upload)
        {
            glBufferData(
                GL_ARRAY_BUFFER,
                mesh.chunk.m_buffer.vertices.size() * sizeof(Primitive::Face::Vertex),
                mesh.chunk.m_buffer.vertices.data(),
                GL_DYNAMIC_DRAW);
        }

        glDrawElements(GL_TRIANGLES, mesh.chunk.m_buffer.vertices.size() / 4u * 6u, GL_UNSIGNED_INT, 0u);
    }
    m_renderQueue.clear();
}