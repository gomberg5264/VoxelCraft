#include "vcpch.hpp"
#include "common/Timer.hpp"

constexpr float serverPeriod = 1.f / 10.f;
constexpr float fixedPeriod = 1.f / 10.f;

const float Timer::serverTickRate = 1.f / serverPeriod;
const float Timer::fixedTickRate = 1.f / fixedPeriod;


float Timer::Elapsed() noexcept 
{ 
    return m_elapsed;
}

float Timer::ElapsedServer() noexcept
{
    return 0.0f;
}

float Timer::ElapsedFixed() noexcept
{
    return 0.0f;
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

    m_server = 0.f;
    m_serverAccum += m_elapsed;
    while (m_serverAccum > serverPeriod)
    {
        m_server += serverPeriod;
        m_serverAccum -= serverPeriod;
    }

    m_fixed = 0.f;
    m_fixedAccum += m_elapsed;
    while (m_fixedAccum > fixedPeriod)
    {
        m_fixed += fixedPeriod;
        m_fixedAccum -= fixedPeriod;
    }
}

void Timer::Reset() noexcept
{
    m_elapsed = 0;
    m_server = 0;
    m_fixed = 0;
    m_serverAccum = 0;
    m_fixedAccum = 0;

    m_total = 0;
    m_frameCount = 0;
    m_clock.restart();
}
