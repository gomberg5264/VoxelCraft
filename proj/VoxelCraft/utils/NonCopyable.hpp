#pragma once

class NonCopyable
{
public:
    NonCopyable() = default;

    NonCopyable(const NonCopyable&) noexcept = delete;
    NonCopyable& operator=(const NonCopyable&) noexcept = delete;
};