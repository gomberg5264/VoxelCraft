#pragma once
#include "common/event/Event.hpp"

class KeyEvent : public Event
{
public:
    KeyEvent(int keyCode)
        : m_keyCode(keyCode) {}

    inline int GetKeyCode() const { return m_keyCode; }

    std::string ToString() const override
    {
        std::stringstream ss;

        ss << GetName() << ": key " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard)

protected:
    int m_keyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
    using KeyEvent::KeyEvent;
    EVENT_CLASS_TYPE(EventType::KeyPress)
};

class KeyReleaseEvent : public KeyEvent
{
public:
    using KeyEvent::KeyEvent;
    EVENT_CLASS_TYPE(EventType::KeyRelease)
};