#include "vcpch.hpp"
#include "client/gl/Buffer.hpp"

VBO::VBO()
{
    glGenBuffers(1, &m_id);
    //std::cout << "+ VBO " << m_id << '\n';
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_id);
    //std::cout << "- VBO " << m_id << '\n';
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

void VBO::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::Unbind() const
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
    //std::cout << "+ VAO " << m_id << '\n';
}

VAO::~VAO()
{
    // Doesn't mind if ID = 0
    glDeleteVertexArrays(1, &m_id);
    //std::cout << "- VAO " << m_id << '\n';
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

void VAO::Bind() const
{
    glBindVertexArray(m_id);
    for (auto& vbo : m_vbos) vbo.Bind();
}

void VAO::Unbind() const
{
    glBindVertexArray(0);
    for (auto& vbo : m_vbos) vbo.Unbind();
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

EBO::EBO()
    : m_elementCount(0)
{
    glGenBuffers(1, &m_id);    
}

EBO::~EBO()
{
    glDeleteBuffers(1, &m_id);
}

EBO::EBO(EBO&& ebo) noexcept
    : m_elementCount(ebo.m_elementCount)
    , m_id(std::exchange(ebo.m_id,0))
{
}

EBO& EBO::operator=(EBO&& ebo) noexcept
{
    if (this == &ebo) return *this;

    m_elementCount = ebo.m_elementCount;
    m_id = ebo.m_id;
}

void EBO::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::SetIndices(const std::vector<unsigned>& indices)
{
    m_elementCount = indices.size();
    Bind();
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned),
        indices.data(), GL_STATIC_DRAW);
    Unbind();
}
