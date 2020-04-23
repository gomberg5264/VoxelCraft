#pragma once
#include <SFML/System/Clock.hpp>

class Timer
{
public:
    static const float serverTickRate;
    static const float fixedTickRate;

    float Elapsed() noexcept;
    float ElapsedServer() noexcept;
    float ElapsedFixed() noexcept;
    float Total() noexcept;
    unsigned FrameCount() noexcept;

    /**
     * Should be called every frame
     */
    void Update() noexcept;
    void Reset() noexcept;

private:
    sf::Clock m_clock;
    float m_elapsed;
    float m_server;
    float m_fixed;

    float m_serverAccum;
    float m_fixedAccum;

    float m_total;
    unsigned m_frameCount;
};