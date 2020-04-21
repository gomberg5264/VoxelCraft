#include "vcpch.hpp"
#include "client/gl/Buffer.hpp"

VBO::VBO(const std::vector<VBO::Element>& elements) noexcept
    : m_elements(elements)
{
}

VBO::VBO(VBO&& rhs) noexcept
    : Buffer(std::move(rhs))
    , m_elements(rhs.m_elements)
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
    : m_elementCount(0)
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

std::unique_ptr<EBO>& VAO::MakeEBO()
{
    m_ebo = std::make_unique<EBO>();
    return m_ebo;
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