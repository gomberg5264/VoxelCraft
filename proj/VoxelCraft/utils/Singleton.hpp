#pragma once

/**
 * TODO: It seams that inheriting from singleton still
 * allows you to create an instance of that object.
 */

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