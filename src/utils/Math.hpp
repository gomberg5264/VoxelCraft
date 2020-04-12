#pragma once

namespace Math
{
    template <typename T>
    inline constexpr T Lerp(const T& t, const T& min, const T& max) noexcept
    {
        assert(min <= max && "Min can't be larger than max");
        assert(t >= T(0) && t <= T(1) && "normalized value should be [0,1]");
        return min + t * (max - min);
    }

    /**
     * Returns a normalized t between min and max
     */
    template <typename T>
    inline constexpr T InverseLerp(const T& val, const T& min, const T& max) noexcept
    {
        assert(val >= min && val <= max && "Val has to be between min and max");
        return (val - min) / (max - min);
    }
}