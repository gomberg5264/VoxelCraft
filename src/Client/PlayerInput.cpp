#include "Client/PlayerInput.h"
#include "Common/GameLayer.h"

#include <Shinobu/Event/MouseEvent.h>
#include <Shinobu/Core/Input/Input.h>
#include <Shinobu/Core/Input/KeyCodes.h>
#include <Shinobu/Core/Input/MouseCodes.h>

void PlayerInput::OnUpdate(sh::Timestep ts)
{
    Player& player = GameLayer::m_players[playerID];
    glm::vec3 offset(0);

    const glm::vec3 forward(player.transform.GetForward());
    const glm::vec3 right(player.transform.GetRight());
    const glm::vec3 up(player.transform.GetUp());

    // NOTE: I flipped the forward for the camera. This is because OpenGL by default 'looks' into the -z
    // direction. Since I reuse the transform class and don't want to modify too many things, I change 
    // the direction for the camera. This is something to keep in mind. It should not affect anything else
    // and we can continue on as we like to.
    if (sh::Input::IsKeyPressed(sh::KeyCode::W)) offset += -forward;
    if (sh::Input::IsKeyPressed(sh::KeyCode::A)) offset += -right;
    if (sh::Input::IsKeyPressed(sh::KeyCode::S)) offset += forward;
    if (sh::Input::IsKeyPressed(sh::KeyCode::D)) offset += right;
    if (sh::Input::IsKeyPressed(sh::KeyCode::E)) offset += up;
    if (sh::Input::IsKeyPressed(sh::KeyCode::Q)) offset += -up;
    
    offset = offset * ts.Seconds() * 5.f;
    callback(std::make_unique<MoveCommand>(player, player.transform.GetPosition() + offset));
}

void PlayerInput::OnEvent(sh::Event& event)
{
    sh::EventDispatcher d(event);

    static bool isRotating = false;
    static glm::vec2 prevPos(0);
    d.Dispatch<sh::MouseButtonPressedEvent>([&](sh::MouseButtonPressedEvent& e)
        {
            if (e.GetButton() == sh::ButtonCode::Right)
            {
                sh::Input::EnableCursor(false);
                isRotating = true;
                prevPos = sh::Input::GetCursorPos();
            }
            return false;
        });
    d.Dispatch<sh::MouseButtonReleasedEvent>([&](sh::MouseButtonReleasedEvent& e)
        {
            if (e.GetButton() == sh::ButtonCode::Right)
            {
                sh::Input::EnableCursor(true);
                isRotating = false;
            }
            return false;
        });

    if (isRotating)
    {
        d.Dispatch<sh::MouseMovedEvent>([&](sh::MouseMovedEvent& e)
            {
                // TODO: Add a sensitivity variable for offset
                /*const */auto offset = glm::vec2(e.GetX(), e.GetY()) - prevPos;
                offset = offset / 25.f * glm::two_pi<float>();

                prevPos = sh::Input::GetCursorPos();


                // TODO: Change this to rotate packet
                GameLayer::m_players[playerID].transform.Rotate(sh::Radians(glm::vec3(-offset.y, -offset.x, 0)));
                return false;
            });
    }
}