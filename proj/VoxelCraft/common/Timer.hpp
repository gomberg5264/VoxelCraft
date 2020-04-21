#pragma once
#include "utils/Singleton.hpp"

class Timer : public Singleton<Timer>
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