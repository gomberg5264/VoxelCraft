#pragma once
#include <string>

/**
* This header contains the event base class that all events inherit from.
*/

/**
 * returns and unsigned which bit is set to true at bit+1
 * TODO: Move this to a core header file.
 */
inline unsigned constexpr Bit(unsigned bit) { return 1 << bit; }

enum class EventType
{
    None = 0,
    
    WindowClose,
};

enum class EventCategory
{
    None = 0,
    Application = Bit(0), // Application events are program related events such as shutdown
    Net = Bit(1)
};

/**
 * These defines generate lookup functions for events
 * All derived classes should call them
 */

#define EVENT_CLASS_TYPE(eventType) \
    static EventType GetStaticType() { return eventType; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #eventType; }

#define EVENT_CLASS_CATEGORY(eventCategory) \
    virtual int GetCategoryFlags() const override { return EventCategory::eventCategory; }

/**
    * Events are used to exchange information between systems. For example, 
    * the window class is responsible for shutting down the system so it needs 
    * to communicate with the engine. The window class also receives input commands
    * which the input manager needs. Or what about the net code? Players need to 
    * receive packets as well. 
    *
    * To decouple these systems, we use an observer pattern. 
    * See the Observer.hpp in utils for more details or check out this site:
    * https://refactoring.guru/design-patterns/observer
    */
class Event
{
public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) const
    {
        return GetCategoryFlags() & static_cast<int>(category);
    }

    inline bool IsHandled() { return m_handled; }
    inline void MakeHandled() { m_handled = true; }

private:
    bool m_handled = false;
};

/**
 * When getting a generic event, we need some way to check it's type.
 * The event dispatcher does that for you. 
 * 
 * You pass the requested type as a template argument and pass the event 
 * object in the constructor. You dispatch the event for every function object. 
 *  
 * It will check the type and call the function if the type is the same as T 
 * and if it is already handled. Being handled means that the event won't continue
 * down whatever chain you implement.
 */
class EventDispatcher
{
public:
    EventDispatcher(Event& event)
        : m_event(event)
    {
    }

    /**
        * Dispatches the event if the function type corresponds with T
        * Returns wether the event type is the same as the function type
        */
    template <typename T, typename F>
    bool Dispatch(const F& func)
    {
        if (m_event.GetEventType() == T::GetStaticType())
        {
            if (!m_event.IsHandled()) func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}