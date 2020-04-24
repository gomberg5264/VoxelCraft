#pragma once
#include "Transform.hpp"
#include "net/packet/Packet.hpp"

class Player
{
public:

    Transform m_transform;
private:

};

Packet& operator<<(Packet& packet, const Player& player)
{
    packet << player.m_transform;
    return packet;
}

Packet& operator>>(Packet& packet, Player& player)
{
    packet >> player.m_transform;
    return packet;
}