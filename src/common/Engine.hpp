#pragma once

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
class Engine
{
public:
    using Time = float;
    
    void Run();

protected:
    void Stop();

private:
    virtual void OnInit() {};
    virtual void OnUpdate(Time time) {}

    bool m_shouldStop;
};