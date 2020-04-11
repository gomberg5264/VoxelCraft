#pragma once

namespace Math
{
    template <typename T>
    inline constexpr T Lerp(const T& t, const T& min, const T& max) noexcept
    {
        assert(min <= max, "Min can't be larger than max");
        assert(t >= T(0) && t <= T(1), "normalized value should be [0,1]");
        return min + t * (max - min);
    }

}