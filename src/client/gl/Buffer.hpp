#pragma once
#include "utils/NonCopyable.hpp"

/**
 * Buffer is the base object for any OpenGL buffers.
 * It defines a constructor and move operators.
 * It is not supposed to be used as a base class.
 * 
 * TODO: Should be const but not sure how to make it work with
 * assignment operator
 */
class Buffer : public NonCopyable
{
public:
    enum class Type
    {
        Vertex = GL_ARRAY_BUFFER,
        Element = GL_ELEMENT_ARRAY_BUFFER
    };

    constexpr Buffer(Type type) noexcept;
    ~Buffer() noexcept;
    Buffer(Buffer&& rhs) noexcept;
    
    void Bind() const noexcept;
    void Unbind() const noexcept;

private:
    const Type m_type;
    unsigned m_id;
};

class VBO : public Buffer
{
public:
    struct Element
    {
        unsigned index;
        unsigned count;
        GLenum dataType;
        GLenum normalized;
        unsigned stride;
        unsigned offset;
    };

    VBO() noexcept;

    //void Upload() const;
    void AddElement(const Element& element);
    const std::vector<Element>& GetElements() const;

private:
    std::vector<Element> m_elements;
};

class EBO : public Buffer
{
public:
    EBO() noexcept;

    void SetIndices(const std::vector<unsigned>& indices);

    size_t m_elementCount;
};

/**
 * Only binds an ebo if it is not zero
 */
class VAO : public NonCopyable
{
public:
    VAO();
    ~VAO();
    VAO(VAO&& vao) noexcept;
    VAO& operator=(VAO&& vao) noexcept;

    void Bind() const;
    void Unbind() const;

    void AddVBO(const std::initializer_list<VBO::Element>& elements);

private:
    std::vector<VBO> m_vbos;
    unsigned m_id;
};