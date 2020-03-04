#pragma once

/**
 * Wraps some data for the buffers
 */
struct Buffer
{
public:
    Buffer(unsigned attribSize, unsigned count)
        : attribSize(attribSize)
        , size(attribSize * count)
        , count(count)
        , buffer(std::make_unique<GLfloat[]>(attribSize * count))
    {
    }

    /**
     * Copy memory from this buffer into another buffer/pointer
     * 
     * @param size the amount of elements in the src buffer
     * @param at where you want to copy the data to in the target buffer
     */
    inline void CopyTo(void* dst) const { memcpy(dst, buffer.get(), size * sizeof(GLfloat)); }
    inline void CopyTo(Buffer& dst, size_t at) const { memcpy(&dst.Data()[at], Data(), size * sizeof(GLfloat)); }
    
    /**
     * Copy memory from another pointer/buffer into this buffer
     *
     * @param size the amount of elements in the src buffer
     * @param at where you want to put the data in this buffer
     */
    inline void CopyFrom(const void* src, size_t at, size_t size) { memcpy(&Data()[at], src, size * sizeof(GLfloat)); }
    inline void CopyFrom(const Buffer& src, size_t at) { memcpy(&Data()[at], src.Data(), src.size * sizeof(GLfloat)); }

    inline GLfloat* Data() { return buffer.get(); }
    inline const GLfloat* Data() const { return buffer.get(); }

    /**
     * Total allocated amount of elements in the buffer
     */
    const unsigned size;

    /**
     * Allocated amount of elements that make up one vertex attrib (3 for pos for example)
     * May refactor to template struct in the future as that may be easier to work with
     */
    const unsigned attribSize;
    
    /**
     * Total allocated amount of vertex attributes in buffer
     */
    const unsigned count;

private:
    std::unique_ptr<GLfloat[]> buffer;
};

struct PosBuffer : public Buffer
{
    PosBuffer(unsigned count) : Buffer(3, count) {}
};

struct TexBuffer : public Buffer
{
    TexBuffer(unsigned count) : Buffer(12, count) {}
};