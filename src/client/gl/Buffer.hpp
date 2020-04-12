#pragma once
#include "utils/NonCopyable.hpp"

class VBO : public NonCopyable
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

    VBO();
    ~VBO();
    VBO(VBO&& vbo) noexcept;
    VBO& operator=(VBO&& vbo) noexcept;

    void Bind() const;
    void Unbind() const;

    void AddElement(const Element& element);
    const std::vector<Element>& GetElements() const;

private:
    std::vector<Element> m_elements;
    unsigned m_id;
};

class EBO : public NonCopyable
{
public:
    EBO();
    ~EBO();
    EBO(EBO&& ebo) noexcept;
    EBO& operator=(EBO&& ebo) noexcept;

    /**
     * If count == 0 this doesn't do anything
     */
    void Bind() const;
    void Unbind() const;
    void SetIndices(const std::vector<unsigned>& indices);

    size_t m_elementCount;

private:
    unsigned m_id;
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