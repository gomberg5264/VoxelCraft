#include "vcpch.hpp"


ChunkMesh::ChunkMesh(unsigned index)
    : m_index(index)
{

}

void ChunkMesh::Generate(const Chunk& chunk)
{
    m_buffer.vertices.clear();
 
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
                
                glm::ivec3 bPos(x, y, z);
                bPos += chunk.GetPos();

                // Add all 6 directions
                for (int i = 0; i < BlockFace::Count; i++)
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


ChunkRenderer::ChunkRenderer()
    : m_shader("res/shaders/face.vert","res/shaders/face.frag")
    , m_elementCount(0)
    , m_ebo(0)
{
    // 6 indices per face, 6 faces per voxel
    m_elementCount = 6u * 6u * chunkSize;

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

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), (void*)(0u * sizeof(float)));
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), (void*)(3u * sizeof(float)));
    //glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), (void*)(5u * sizeof(float)));
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);
    //
    //glBindVertexArray(0);
}

ChunkRenderer::~ChunkRenderer()
{
    glDeleteBuffers(1, &m_ebo);
}

unsigned ChunkRenderer::GenerateIndex()
{
    m_vaos.emplace_back();
    auto& vao = m_vaos.back();
    vao.AddVBO(
        {
            { 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
            { 1, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
            { 2, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 5u * sizeof(float) },
        });

    vao.Bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    vao.Unbind();

        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), (void*)(0u * sizeof(float)));
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), (void*)(3u * sizeof(float)));
        //glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), (void*)(5u * sizeof(float)));

    //UpdateIndicesBuffer(m_vaos.size());
    return m_vaos.size() - 1u;
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
//  glBindVertexArray(m_vao);
    m_shader.Use();

    for (const auto& mesh : m_renderQueue)
    {
        auto& vao = m_vaos[mesh.mesh.m_index];
        //glBindBuffer(GL_ARRAY_BUFFER, m_vbos[mesh.mesh.m_index]);

        vao.Bind();

        if (mesh.upload)
        {
            glBufferData(
                GL_ARRAY_BUFFER,
                mesh.mesh.m_buffer.vertices.size() * sizeof(Primitive::Face::Vertex),
                mesh.mesh.m_buffer.vertices.data(),
                GL_DYNAMIC_DRAW);
        }

        glDrawElements(GL_TRIANGLES, mesh.mesh.m_buffer.vertices.size() / 4u * 6u, GL_UNSIGNED_INT, 0);
    }
    m_renderQueue.clear();
}
