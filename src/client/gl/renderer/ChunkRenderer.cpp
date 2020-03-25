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
    m_drawCount = 0;
}

ChunkMesh::ChunkMesh(unsigned index)
    : m_index(index)
{
}

void ChunkMesh::Generate(const Chunk& chunk)
{
    m_buffer = std::move(Primitive::Face::MakeBuffer(
        BlockFace::Front, 
        chunk.GetPos().x, 
        chunk.GetPos().y, 
        chunk.GetPos().z, 
        0));
}
