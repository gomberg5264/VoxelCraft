#pragma once
#include <SFML/System/Clock.hpp>

class Timer
{
public:
    float Elapsed() noexcept;
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
    float m_total;
    unsigned m_frameCount;
};