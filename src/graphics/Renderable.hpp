#pragma once

class Renderable
{
public:
    virtual glm::mat4 ToWorld() const { return glm::mat4(1); };

    // This data is used by the renderer
    // The format is dependent on the renderer implementation
    virtual const VBO& GetDrawData() const noexcept = 0;
};