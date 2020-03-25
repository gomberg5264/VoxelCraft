#include "vcpch.hpp"

ChunkRenderer::ChunkRenderer(unsigned chunkCount)
    : m_shader("res/shaders/face.vert","res/shaders/face.frag")
    , m_chunkCount(chunkCount)
    , m_drawCount(0)
{
    // Create buffers
    glGenVertexArrays(1, &m_buffer.vao);
    glBindVertexArray(m_buffer.vao);

    glGenBuffers(1, &m_buffer.vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.vertices);
    glBufferData(
        GL_ARRAY_BUFFER, 
        4u * sizeof(Primitive::Face::Vertex) * chunkSize * chunkCount, 
        nullptr, 
        GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_buffer.indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.indices);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        6u * sizeof(unsigned) * chunkSize * chunkCount, 
        nullptr, 
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex),        (void*)(0u * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex),        (void*)(3u * sizeof(float)));
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Primitive::Face::Vertex), (void*)(5u * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

ChunkRenderer::~ChunkRenderer()
{
    glDeleteBuffers(1, &m_buffer.vertices);
    glDeleteBuffers(1, &m_buffer.indices);
    glDeleteBuffers(1, &m_buffer.vao);
}

void ChunkRenderer::SetVP(const glm::mat4& vp)
{
    m_shader.Use();
    m_shader.SetMatrix("aVP", glm::value_ptr(vp));
}

void ChunkRenderer::Render(const ChunkMesh& mesh, bool updateDrawData)
{
    //m_renderQueue.push_back(mesh);
    m_drawCount += mesh.m_buffer.indices.size();
    if (updateDrawData) m_updateQueue.push_back(mesh);
}

void ChunkRenderer::Display()
{
    glBindVertexArray(m_buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.vertices);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.indices);
    m_shader.Use();

    for (const auto& mesh : m_updateQueue)
    {
        //glBufferData(
        //    GL_ARRAY_BUFFER,
        //    mesh.get().m_buffer.vertices.size() * sizeof(Primitive::Face::Vertex),
        //    mesh.get().m_buffer.vertices.data(),
        //    GL_DYNAMIC_DRAW);

        //glBufferData(
        //    GL_ELEMENT_ARRAY_BUFFER,
        //    mesh.get().m_buffer.indices.size() * sizeof(unsigned),
        //    mesh.get().m_buffer.indices.data(),
        //    GL_DYNAMIC_DRAW);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            mesh.get().m_index * 4u * 6u * chunkSize * sizeof(Primitive::Face::Vertex),
            mesh.get().m_buffer.vertices.size() * sizeof(Primitive::Face::Vertex),
            mesh.get().m_buffer.vertices.data());

        glBufferSubData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.get().m_index * 6u * chunkSize * sizeof(unsigned),
            mesh.get().m_buffer.indices.size()* sizeof(unsigned),
            mesh.get().m_buffer.indices.data());
    }
    m_updateQueue.clear();

    glDrawElements(GL_TRIANGLES, m_drawCount,GL_UNSIGNED_INT,0);

    // TODO would be nice if I could add an offset of 6 after 6 passes
    // since the order is always the same
    //glDrawElementsBaseVertex(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0,0);
    m_drawCount = 0;
}

ChunkMesh::ChunkMesh(unsigned index)
    : m_index(index)
{
}

void ChunkMesh::Generate(const Chunk& chunk)
{
    m_buffer.indices.clear();
    m_buffer.vertices.clear();
    const auto& blocks = chunk.GetBlockArray();
    for (unsigned y = 0; y < chunkDimension.y; y++)
        for (unsigned z = 0; z < chunkDimension.z; z++)
            for (unsigned x = 0; x < chunkDimension.x; x++)
            {
                const auto& bData = BlockDataFactory::GetInstance().GetBlockData(blocks[x][y][z]);
                glm::ivec3 bPos(x, y, z);
                bPos += chunk.GetPos();

                // Add all 6 directions
                for (int i = 0; i < BlockFace::Count; i++)
                {
                    auto buffer = Primitive::Face::MakeBuffer(
                        BlockFace(i), bPos.x, bPos.y, bPos.z, bData.texture[i]);
                    
                    const size_t indexOffset = m_buffer.vertices.size();
                    std::transform(
                        buffer.indices.begin(),
                        buffer.indices.end(),
                        buffer.indices.begin(),
                        [indexOffset](unsigned index) {return index + indexOffset; });

                    m_buffer.indices.insert(
                        m_buffer.indices.end(), 
                        buffer.indices.begin(), 
                        buffer.indices.end());

                    m_buffer.vertices.insert(
                        m_buffer.vertices.end(), 
                        buffer.vertices.begin(), 
                        buffer.vertices.end());                    
                }

                return;
            }
    
    
    m_buffer.indices.clear();
    m_buffer.vertices.clear();
    m_buffer = std::move(Primitive::Face::MakeBuffer(
        BlockFace::Front, 
        chunk.GetPos().x, 
        chunk.GetPos().y, 
        chunk.GetPos().z, 
        0));
}
