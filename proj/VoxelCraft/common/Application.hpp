#pragma once
#include "common/event/Event.hpp"
#include "common/Timer.hpp"

#include "utils/Observer.hpp"

#include <vector>

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

class Layer;

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
    using Layers = std::vector<std::unique_ptr<Layer>>;

    void Run() noexcept;

private:
    Layers m_layers;
};

/**
 * A layer is a system that the application will execute
 * You can hook into this behavior by overriding certain virtual functions
 *
 * OnInit
 * OnUpdate
 * OnNotify
 *
 */
class Layer : public Subscriber<Event>
{
public:
    virtual ~Layer() { OnDeinit(); }

    void Init(Application& app);
    void Update();
    
    inline Application& GetApplication() noexcept { return *m_app; }
    inline bool ShouldExit() { return m_shouldExit; }
    inline void Exit() { m_shouldExit = true; }


private:
    virtual void OnInit() {};
    virtual void OnUpdate() {};
    virtual void OnDeinit() {};

    Application* m_app;
    bool m_shouldExit = false;
};

extern void CreateApplication(Application::Layers& layers);