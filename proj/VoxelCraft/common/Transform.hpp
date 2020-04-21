#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform
{
public:
    inline static constexpr glm::vec3 Forward() noexcept { return glm::vec3(0, 0, 1); }
    inline static constexpr glm::vec3 Right() noexcept { return glm::vec3(1, 0, 0); }
    inline static constexpr glm::vec3 Up() noexcept { return glm::vec3(0, 1, 0); }

    inline glm::mat4 ToWorld() const noexcept
    {
        const auto roll = glm::rotate(glm::mat4(1), m_euler.z, Forward());
        const auto pitch = glm::rotate(glm::mat4(1), m_euler.x, Right());
        const auto yaw = glm::rotate(glm::mat4(1), m_euler.y, Up());
        const auto scale = glm::scale(glm::mat4(1), m_scale);
        const auto translate = glm::translate(glm::mat4(1), m_pos);

        return translate * pitch * yaw * roll  * scale;
    }

    glm::vec3 m_pos{ 0 };
    glm::vec3 m_scale{ 1 };
    glm::vec3 m_euler{ 0 };
};