#pragma once
#include <SFML/Network/IpAddress.hpp>

struct Address
{
    sf::IpAddress ip;
    unsigned short port;
};

inline bool operator==(const Address& lhs, const Address& rhs)
{
    return lhs.ip == rhs.ip && lhs.port == rhs.port;
}

inline std::ostream& operator<<(std::ostream& os, const Address& address)
{
    os << address.ip.toString() << ':' << address.port;
    return os;
}
