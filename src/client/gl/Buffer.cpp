#include "vcpch.hpp"

VBO::VBO()
{
    glGenBuffers(1, &m_id);
    std::cout << "+ VBO " << m_id << '\n';
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_id);
    std::cout << "- VBO " << m_id << '\n';
}

VBO::VBO(VBO&& vbo) noexcept
    : m_elements(std::move(vbo.m_elements))
    , m_id(std::exchange(vbo.m_id,0))
{
}

VBO& VBO::operator=(VBO&& vbo) noexcept
{
    m_elements = std::move(vbo.m_elements);
    m_id = std::exchange(vbo.m_id, 0);

    return *this;
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::AddElement(const Element& element)
{
    m_elements.push_back(element);
}

const std::vector<VBO::Element>& VBO::GetElements() const
{
    return m_elements;
}

VAO::VAO()
{
    glGenVertexArrays(1, &m_id);
    std::cout << "+ VAO " << m_id << '\n';
}

VAO::~VAO()
{
    // Doesn't mind if ID = 0
    glDeleteVertexArrays(1, &m_id);
    std::cout << "- VAO " << m_id << '\n';
}

VAO::VAO(VAO&& vao) noexcept
    : m_vbos(std::move(vao.m_vbos))
    , m_id(std::exchange(vao.m_id,0))
{
}

VAO& VAO::operator=(VAO&& vao) noexcept
{
    m_vbos = std::move(vao.m_vbos);
    m_id = std::exchange(vao.m_id, 0);

    return *this;
}

void VAO::Bind()
{
    glBindVertexArray(m_id);
    for (auto& vbo : m_vbos) vbo.Bind();
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::AddVBO(const std::initializer_list<VBO::Element>& elements)
{
    Bind();

    m_vbos.emplace_back();
    auto& vbo = m_vbos.back();
    for (const auto& elem : elements)
        vbo.AddElement(elem);

    vbo.Bind();
    for (const auto& elem : vbo.GetElements())
    {
        glEnableVertexAttribArray(elem.index);
        glVertexAttribPointer(
            elem.index,
            elem.count,
            elem.dataType,
            elem.normalized,
            elem.stride,
            (void*)elem.offset
            );
    }
    vbo.Unbind();
}
