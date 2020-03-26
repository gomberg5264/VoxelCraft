#pragma once


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