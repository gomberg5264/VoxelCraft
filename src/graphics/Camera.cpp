#include "vcpch.hpp"

Camera::Camera(sf::Window& window)
    : m_window(window)
{
}

void Camera::Update(float dt)
{
    if (!m_window.hasFocus())
        return;

    glm::vec3 movement = glm::vec3();

    // Movement
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) movement.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) movement.y -= 1;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.z += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.z -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1;

        movement *= m_speed;
    }

    m_eye += movement;

    // Rotation
    /*
    {
        static double px = -1;
        static double py = -1;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (px == -1)
        {
            px = x;
            py = y;
        }

        double dX = x - px;
        double dY = y - py;
        px = x;
        py = y;

        float rotX = 0.f;
        float rotY = 0.f;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            rotX = dX * 0.3f * 0.025f;
            rotY = dY * 0.2f * 0.025f;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    

        if (rotX != 0 || rotY != 0 || dot(movement, movement) != 0.f)
        {
            renderer.OnMove();
        }

        camera = camera * mat4::RotateX(rotY) * mat4::RotateY(rotX) * mat4::Translate(movement * speed * clamp(dt, 0.f, 1.f));
    }
    */
}

glm::mat4 Camera::GetProjection() const
{
    return glm::perspective<float>(glm::radians(45.f), m_window.getSize().x / m_window.getSize().y, 0.1f, 100.f);
}

glm::mat4 Camera::GetView() const
{
    return glm::lookAt(m_eye, m_target,glm::fvec3(0,1,0));
}
