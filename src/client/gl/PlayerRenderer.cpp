#include "vcpch.hpp"
#include "PlayerRenderer.hpp"

PlayerMesh::PlayerMesh(Transform& transform)
    : m_shader("res/shaders/face.vert", "res/shaders/face.frag")
    , m_transform(transform)
    , m_vao(
        {
            { 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
            { 1, 3, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
            { 2, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 6u * sizeof(float) },
            { 3, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 8u * sizeof(float) },
        })
{
    m_shader.Bind();
    
    m_vao.Bind();
    m_vao.m_ebo = std::make_unique<EBO>();
    m_vao.m_ebo->Bind();
    m_vao.m_ebo->Upload(Primitive::Face::MakeIndices(6));

    std::vector<Primitive::Face::Vertex> buffer;
    for (int i = 0; i < 6; i++)
    {
        auto vertices = Primitive::Face::MakeBuffer(BlockFace(i), 0, 0, 0, i % 4).vertices;
        buffer.insert(buffer.end(), vertices.begin(), vertices.end());
    }

    m_vao.m_vbo.Bind();
    m_vao.m_vbo.Upload(buffer);
//    glBufferData(
//        GL_ARRAY_BUFFER, 
//        buffer.size() * sizeof(Primitive::Face::Vertex), 
//        buffer.data(), GL_STATIC_DRAW);  
    m_vao.m_vbo.Unbind();
    m_vao.Unbind();
}

void PlayerMesh::Draw(const Camera& camera)
{
    m_vao.Bind();

    m_shader.Bind();
    //m_shader.SetMatrix("aVP", glm::value_ptr(camera.GetProjection() * camera.GetView()));
    //m_shader.SetMatrix("aModel", glm::value_ptr(m_transform.ToWorld()));
    auto model = glm::mat4(1);// glm::translate(glm::vec3(0, 20, -4));

    m_shader.SetMatrix("aVP", glm::value_ptr(camera.GetProjection() * camera.GetView()));
    m_shader.SetMatrix("aModel", glm::value_ptr(m_transform.ToWorld()));

    glDrawElements(GL_TRIANGLES, m_vao.m_ebo->ElementCount(), GL_UNSIGNED_INT, 0);
    m_vao.Unbind();
}