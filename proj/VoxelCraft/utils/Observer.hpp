#pragma once
#include "utils/NonCopyable.hpp"

#include <vector>
#include <cassert>

/**
 * The observer pattern allows us to dynamically add callbacks
 * to a systems without the systems having to care about the 
 * callback's implementation details. This means that the systems
 * are nicely decoupled.
 * 
 * TODO: System assumes that publisher does not change
 * memory address. This is needed so that the subscriber
 * unsubscribes itself.
 * Add move semantics to publishers so that it can update
 * the reference to it and not result into a dangling pointer.
 *
 * Basically, if either publisher or subscriber changes memory
 * address, we will have dangling pointers. We may want to look
 * into preventing such a situation.
 */

template <typename T> class Subscriber;
/**
 * A publisher holds a list of subscribers. 
 * It calls T on every subscriber. To use it, either make it 
 * a variable or inherit from it.
 *
 * TODO: I don't think that publisher is allowed to copy.
 * what does it mean to copy subscribers? Verify that and 
 * see if we run into issues.
 */
template <typename T>
class Publisher : public NonCopyable
{
public:
    virtual ~Publisher() 
    {
        for (auto& sub : m_subscribers)
            sub.get().Unsubscribe();
    }

    inline void Subscribe(Subscriber<T>& sub)
    {
        m_subscribers.push_back(sub);
        sub.OnSubscribe(*this);
    }
    inline void Unsubscribe(Subscriber<T>& sub)
    {
        m_subscribers.erase(std::remove_if(m_subscribers.begin(), m_subscribers.end(),
            [&sub](const std::reference_wrapper<Subscriber<T>>& match) { return &sub == &match.get(); }));
    }

    inline void Notify(T&& value)
    {
        for (auto& sub : m_subscribers)
            sub.get().OnNotify(value);
    }
private:
    std::vector<std::reference_wrapper<Subscriber<T>>> m_subscribers;
};

/**
 * A subscriber can register to a publisher and push events to it
 */
template <typename T>
class Subscriber
{
public:
    virtual ~Subscriber() { m_publisher->Unsubscribe(*this); }

    /**
     * Called by the publisher when subscribing
     */
    inline void OnSubscribe(Publisher<T>& publisher) { m_publisher = &publisher; }
    /**
     * Unsubscribes from the publisher if it is registered to one
     */
    inline void Unsubscribe()
    {
        if (m_publisher)
        {
            m_publisher->Unsubscribe(*this);
            m_publisher = nullptr;
        }
    }

    /**
     * Publish something to the publisher. All subscribers
     * will be notified with value
     */
    void Publish(T&& value)
    {
        assert(m_publisher != nullptr && "You first have to subscribe to a publisher");
        m_publisher->Notify(std::move(value));
    }
    virtual void OnNotify(T& value) = 0;

private:
    Publisher<T>* m_publisher = nullptr;
};