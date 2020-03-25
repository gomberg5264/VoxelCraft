#pragma once

template <typename T>
class Singleton
{
public:
    static T& GetInstance() noexcept
    {
        static T t;
        return t;
    }

protected:
    Singleton() = default;
};