#pragma once
#include <string>

#include "utils/Observer.hpp"
#include "common/event/WindowEvent.hpp"
#include "common/event/InputEvent.hpp"

/**
 * The window sets up a render context and forwards input and window events
 */
class Window
{
public:
    struct Config
    {
        unsigned x{ 1280 };
        unsigned y{ 720 };

        std::string title{ "VoxelCraft" };
    };

    Window(Config config = {});

    void SetCursorGrabbed(bool state);
    void SetCursorVisible(bool state);
    /**
     * pos From top left corner
     */
    void SetMousePos(const sf::Vector2i& pos) const;

    sf::Vector2u GetSize() const;
    sf::Vector2i GetMousePos() const;
    bool HasFocus() const;

    void Close();
    void Clear();
    void Display();
    void PollEvents(Publisher<Event>& publisher);

private:
    sf::Window m_window;
};