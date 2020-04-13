#pragma once

class Camera
{
public:
    Camera(sf::Window& window);

    virtual void Update(float dt) = 0;

    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;
    glm::mat4 GetRotation() const;

    glm::fvec3 m_target{ 0,0,1 };
    glm::fvec3 m_eye{ 0 };
    sf::Window& m_window;
};

class FreelookCamera : public Camera
{
public:
    FreelookCamera(sf::Window& window);
    
    virtual void Update(float dt) override final;

    float m_sensitivity{ 0.2f };
    float m_speed{ 1.f };

private:

    glm::vec2 m_euler{ 0.f,-90.f };
    sf::Vector2i m_oldPos;
};