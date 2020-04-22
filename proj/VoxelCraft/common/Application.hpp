#pragma once
#include "common/event/Event.hpp"
#include "common/Timer.hpp"

#include "utils/Observer.hpp"

/**
 * A namespace for core modules that I want to access at many places
 * Not sure if this is the way to go though, I did this because it is nice
 * to sometimes get the time for debug purposes and when I am in doubt, I just
 * refer to Unity's API
 */
namespace Core
{
    extern Timer time;
}

/**
 * A simple framework that our project runs in
 * If needed this will be changed into a class that handles
 * events between layers. Layers can be user specified.
 * It would essentially be an event system which is usefull 
 * because we need to register graphics events (window close) 
 * to the actual runtime if we want to decouple these
 *
 * But that is overkill for the time being
 */
class Application : public Publisher<Event>
{
public:
    Application();
    
    void Run() noexcept;
};

/**
 * The user can define some runtime stuff with this class
 */
class Layer : public Subscriber<Event>
{
public:
    void Init(Application& app);
    void Update();
    
    inline Application& GetApplication() noexcept { return *m_app; }
    inline bool ShouldExit() { return m_shouldExit; }
    inline void Exit() { m_shouldExit = true; }

private:
    virtual void OnInit() {};
    virtual void OnUpdate() = 0;
    
    Application* m_app;
    bool m_shouldExit = false;
};

extern std::unique_ptr<Layer> CreateApplication();