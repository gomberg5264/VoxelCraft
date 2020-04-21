#include "vcpch.hpp"
#include "common/Timer.hpp"

float Timer::Elapsed() noexcept 
{ 
    return m_elapsed;
}

float Timer::Total() noexcept 
{ 
    return m_total;
}

unsigned Timer::FrameCount() noexcept
{
    return m_frameCount;
}

void Timer::Update() noexcept 
{ 
    m_elapsed = m_clock.restart().asSeconds();
    m_total += m_elapsed;
    m_frameCount++;
}

void Timer::Reset() noexcept
{
    m_elapsed = 0;
    m_total = 0;
    m_frameCount = 0;
    m_clock.restart();
}
