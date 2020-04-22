#pragma once
#include <type_traits>

/**
 * Strongly typed enums with bit mask operations
 * http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
 */

template<typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

#define ENABLE_BITMASK_OPERATORS(x)\
    template<> \
    struct EnableBitMaskOperators<x> \
    { \
        static const bool enable = true; \
    };

// Define all operator overloads
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ~(Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        ~static_cast<underlying>(rhs)
        );
}

#define GENERATE_BITMASK_OPERATOR(x) \
template<typename Enum> \
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type \
operator x(Enum lhs, Enum rhs) \
{ \
    using underlying = typename std::underlying_type<Enum>::type; \
    return static_cast<Enum> ( \
        static_cast<underlying>(lhs) x \
        static_cast<underlying>(rhs) \
        ); \
}

GENERATE_BITMASK_OPERATOR(|)
GENERATE_BITMASK_OPERATOR(&)
GENERATE_BITMASK_OPERATOR(^)
GENERATE_BITMASK_OPERATOR(|=)
GENERATE_BITMASK_OPERATOR(&=)
GENERATE_BITMASK_OPERATOR(^=)