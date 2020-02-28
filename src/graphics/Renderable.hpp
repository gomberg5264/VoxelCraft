#pragma once

class Renderer;

class Renderable
{
    friend Renderer;
public:

    const glm::mat4& GetToWorld() const;

protected:
    glm::mat4 m_toWorld;

private:

    /**
     * Receives a bound buffer. 
     */
    virtual void LoadVBO() = 0;
    
    /**
     * Used for the draw call
     */
    virtual unsigned VertexCount() const = 0;
};

class Cube : public Renderable
{
    virtual void LoadVBO() override final;
    virtual unsigned VertexCount() const override final;
};