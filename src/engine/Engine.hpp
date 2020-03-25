#pragma once

/**
 * A simple runtime instance
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