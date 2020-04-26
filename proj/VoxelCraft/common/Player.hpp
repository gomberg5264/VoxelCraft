#pragma once
#include "common/Transform.hpp"

#include "net/packet/Packet.hpp"

class Player
{
public:
    unsigned m_id;

    Transform m_transform;
};

inline sf::Packet& operator<<(sf::Packet& p, const Player& player)
{
    p << player.m_id << player.m_transform;
    return p;
}

inline sf::Packet& operator>>(sf::Packet& p, Player& player)
{
    p >> player.m_id >> player.m_transform;
    return p;
}