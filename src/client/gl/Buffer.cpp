#include "vcpch.hpp"
#include "client/gl/Buffer.hpp"

constexpr Buffer::Buffer(Type type) noexcept
    : m_type(type)
    , m_id(0)
{
    glGenBuffers(1, &m_id);
}

Buffer::~Buffer() noexcept
{
    if (m_id != 0)
        glDeleteBuffers(1, &m_id);
}

Buffer::Buffer(Buffer&& rhs) noexcept
    : m_type(rhs.m_type)
    , m_id(std::exchange(rhs.m_id, 0))
{
}

void Buffer::Bind() const noexcept
{
    glBindBuffer(static_cast<GLenum>(m_type), m_id);
}

void Buffer::Unbind() const noexcept
{
    glBindBuffer(static_cast<GLenum>(m_type), 0);
}

//VBO::VBO(VBO&& vbo) noexcept
//    : m_elements(std::move(vbo.m_elements))
//    , m_id(std::exchange(vbo.m_id,0))
//{
//}
//
//VBO& VBO::operator=(VBO&& vbo) noexcept
//{
//    m_elements = std::move(vbo.m_elements);
//    m_id = std::exchange(vbo.m_id, 0);
//
//    return *this;
//}

VBO::VBO() noexcept
    : Buffer(Type::Vertex)
{
}

void VBO::AddElement(const Element& element)
{
    m_elements.push_back(element);
}

const std::vector<VBO::Element>& VBO::GetElements() const
{
    return m_elements;
}

EBO::EBO() noexcept
    : Buffer(Type::Element)
    , m_elementCount(0)
{    
}
//
//EBO::EBO(EBO&& rhs) noexcept
//    : Buffer(std::move(rhs))
//    , m_elementCount(rhs.m_elementCount)
//{
//}

//EBO::EBO(EBO&& ebo) noexcept
//    : m_elementCount(ebo.m_elementCount)
//    , m_id(std::exchange(ebo.m_id,0))
//{
//}

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
    , m_id(std::exchange(vao.m_id, 0))
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
