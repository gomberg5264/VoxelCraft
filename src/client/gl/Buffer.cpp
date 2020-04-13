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

VBO::VBO(const std::initializer_list<VBO::Element>& elements) noexcept
    : Buffer(Type::Vertex)
    , m_elements(elements)
{
}

void VBO::Setup()
{
    Bind();

    for (const auto& elem : m_elements)
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
    Unbind();
}

EBO::EBO() noexcept
    : Buffer(Type::Element)
    , m_elementCount(0)
{
}

size_t EBO::ElementCount() const noexcept
{
    return m_elementCount;
}

VAO::VAO(VAO&& vao) noexcept
    : m_vbo(std::move(vao.m_vbo))
    , m_id(std::exchange(vao.m_id, 0))
{
}

VAO::VAO(VBO&& vbo) noexcept
    : m_vbo(std::move(vbo))
{
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
    m_vbo.Setup();
    glBindVertexArray(0);
}

VAO::~VAO()
{
    if (m_id != 0) glDeleteVertexArrays(1, &m_id);
}

void VAO::Bind() const
{
    glBindVertexArray(m_id);
    if (m_ebo) m_ebo->Bind();
}

void VAO::Unbind() const
{
    glBindVertexArray(0);
    if (m_ebo) m_ebo->Unbind();
}