#pragma once
#include "utils/NonCopyable.hpp"

#include <glad/glad.h>

#include <vector>
#include <memory>

/**
 * Buffer is the base object for any OpenGL buffers.
 * It defines a constructor and move operators.
 * It is not supposed to be used as a base class.
 * 
 * TODO: Should be const but not sure how to make it work with
 * assignment operator
 */
template <GLenum bufferType>
class Buffer : public NonCopyable
{
public:
    constexpr Buffer() noexcept : m_id(0) { glGenBuffers(1, &m_id); }
    Buffer(Buffer&& rhs) noexcept : m_id(std::exchange(rhs.m_id, 0)) {}
    ~Buffer() noexcept { if (m_id != 0) glDeleteBuffers(1, &m_id); }
    
    void Bind() const noexcept { glBindBuffer(bufferType, m_id); }
    void Unbind() const noexcept{ glBindBuffer(bufferType, 0); }

private:
    unsigned m_id;
};

class VAO;
/**
 * A wrapper for a buffer object
 */
class VBO : public Buffer<GL_ARRAY_BUFFER>
{
    friend VAO;
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

    VBO(const std::vector<VBO::Element>& elements) noexcept;
    VBO(VBO&& rhs) noexcept;

    /**
     * Assumes a vector but we use begin so that we can also
     * use initializer lists
     */
    template<typename T>
    void Upload(const T& attributes) noexcept
    {
        Bind();
        glBufferData(
            GL_ARRAY_BUFFER,
            attributes.size() * sizeof(*attributes.begin()),
            &(*attributes.begin()),
            GL_DYNAMIC_DRAW);
    }
    
private:

    /**
     * This function will be called from the VAO
     * It should only be called when a valid VAO is bound
     * It sets up the vertex attribute pointers
     */
    void Setup();
    
    /**
     * Using initizalizer lists did some very weird things with move constructor
     * https://tristanbrindle.com/posts/beware-copies-initializer-list
     */
    const std::vector<Element> m_elements;
};

class EBO : public Buffer<GL_ELEMENT_ARRAY_BUFFER>
{
public:
    EBO() noexcept;

    template<typename T>
    void Upload(const T& indices) noexcept
    {
        Bind();
        m_elementCount = indices.size();
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(*indices.begin()),
            &(*indices.begin()),
            GL_STATIC_DRAW);
    }

    size_t ElementCount() const noexcept;

private:
    size_t m_elementCount;
};

/**
 * A vertex array object wrapper for OpenGL
 *
 * The reason why we call Setup is because we have to bind the 
 * vertex attribute information after we have bound the vao.
 * To adhere to RAII, we have to pass the vbo in the constructor
 * 
 * You can modify the buffers by simply binding them
 * For drawing, you only have to bind the VAO.
 *
 * TODO: Right now, you can only use one vbo
 * TODO: Chunk renderer defines one ebo for all chunk vbos.
 * Instead of checking ebo, make a unique class for it.
 */
class VAO : public NonCopyable
{
public:
    VAO(VBO&& vbo) noexcept;
    VAO(VAO&& vao) noexcept;
    ~VAO() noexcept;

    std::unique_ptr<EBO>& MakeEBO();

    /**
     * This only binds the VAO. 
     * Binding a VAO means that all buffer address information is stored.
     * This regards the vertex attrib pointers. It stores where that data is.
     * This means that vbo will not be binded when calling this, it doesn't need to.
     * This does mean that the ebo will be binded (if one exists).
     * 
     * https://stackoverflow.com/questions/17332657/does-a-vao-remember-both-a-ebo-ibo-elements-or-indices-and-a-vbo
     */
    void Bind() const;
    void Unbind() const;

    VBO m_vbo;

    /**
     * Be sure that when you create an EBO, you also bind it
     */
    std::unique_ptr<EBO> m_ebo;

private:
    unsigned m_id;
};