#pragma once

/**
 * Wraps some data for the buffers
 */
class VBO
{
public:
    struct Vertex
    {
        float pos[3];
        int texIndex[6];
    };

    VBO(size_t size) 
        : m_data(std::make_unique<Vertex[]>(size))
        , m_size(size)
        , m_activeSize(0)
    {}

    VBO(VBO&& vbo) noexcept
        : m_size(vbo.m_size)
        , m_activeSize(vbo.m_activeSize)
    {
        m_data.swap(vbo.m_data);
    }

    VBO& operator=(VBO && vbo) noexcept
    {
        m_size = vbo.m_size;
        m_activeSize = vbo.m_activeSize;
        m_data.swap(vbo.m_data);
    }

    inline void Resize(size_t size)
    {
        auto temp = std::make_unique<Vertex[]>(size);
        memcpy(&temp.get()[0], Data(), size * sizeof(Vertex));

        m_data.swap(temp);
        m_size = size;
    }

    inline void WriteBack(const Vertex& vertex)
    {
        (*this)[m_activeSize++] = vertex;
    }

    // The amount of elements that actually contain data
    inline void SetActiveSize(size_t size) noexcept { m_activeSize = size; }
    inline size_t GetActiveSize() const noexcept { return m_activeSize; }
    inline size_t GetSize() const noexcept { return m_size; }

    inline Vertex* Data() { return &m_data.get()[0]; }
    inline const Vertex* Data() const { return &m_data.get()[0]; }

    inline Vertex& operator[] (size_t index) 
    { assert(index > 0 && index < m_size && "Out of bounds"); return m_data.get()[index]; }
    inline const Vertex& operator[] (size_t index) const { return (*this)[index]; }
private:
    std::unique_ptr<Vertex[]> m_data;
    unsigned m_size; 
    unsigned m_activeSize;
};