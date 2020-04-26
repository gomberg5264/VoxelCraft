#pragma once
#include "utils/Bitmask.hpp"

#include <sstream>
#include <functional>

/**
 * returns and unsigned which bit is set to true at bit+1
 */
inline unsigned constexpr Bit(unsigned bit) { return 1 << bit; }

enum class EventType
{
    None = 0,
    
    WindowClose, WindowResize,
    
    // These are standard events that all games may use
    NetConnect, NetConnectResponse, NetDisconnect,
    NetHost, NetHostResponse, NetShutdown,
    NetJoined, 
    
    // There are generic net events
    NetClientPacketSend, NetClientPacketReceive,
    NetServerPacketSend, NetServerPacketReceive,

    KeyPress, KeyRelease,
};

/**
 * http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
 */
enum class EventCategory : unsigned
{
    None = 0,
    Window = Bit(0), // Window events such as key presses, resize, closing

    Gameplay = Bit(1), 
    Net = Bit(2), // Netcode packets such as handshakes, connect, movement.

    Input = Bit(3),
    Keyboard= Bit(4),
};
ENABLE_BITMASK_OPERATORS(EventCategory)


/**
 * These defines generate lookup functions for events
 * All derived classes should call them
 */
#define EVENT_CLASS_TYPE(eventType) \
    static EventType GetStaticType() { return eventType; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #eventType; }

#define EVENT_CLASS_CATEGORY(eventCategory) \
    virtual EventCategory GetCategoryFlags() const override { return eventCategory; }


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
    virtual EventCategory GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) const
    {
        return static_cast<unsigned>(GetCategoryFlags() & category) != 0;
    }

    inline bool IsHandled() { return m_handled; }
    inline void MakeHandled() { m_handled = true; }

private:
    bool m_handled = false;
};

/**
 * Call this macro in the dispatch function in EventDispatcher
 * if you want to bind a member function in the func call
 */
#define BIND(fn) std::bind(&fn, this, std::placeholders::_1)

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
     *
     * func is expected to take an event reference and return void
     */
    template <typename T, typename F>
    bool Dispatch(const F& func)
    {
        static_assert(std::is_base_of<Event, T>::value, "T is not derived from event");
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