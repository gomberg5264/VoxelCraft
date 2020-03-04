#pragma once

class Renderable
{
public:
    struct BufferData
    {
        const GLfloat* buffer;
        const size_t size;
    };

    virtual glm::mat4 ToWorld() const { return glm::mat4(1); };

    // This data is used by the renderer
    // The format is dependent on the renderer implementation
    virtual const BufferData GetPosData() const noexcept = 0;
    virtual const BufferData GetTextureData() const noexcept = 0;

    virtual const unsigned GetDrawCount() const noexcept { return 1; }
};