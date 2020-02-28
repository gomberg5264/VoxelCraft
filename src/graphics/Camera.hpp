#pragma once

class Camera
{
public:
    Camera(sf::Window& window);

    virtual void Update(float dt);

    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;

    glm::fvec3 m_target{ 0,0,1 };
    glm::fvec3 m_eye{ 0 };
    float m_speed{ 1 };

    sf::Window& m_window;
};