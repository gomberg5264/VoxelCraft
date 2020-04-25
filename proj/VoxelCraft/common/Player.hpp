#pragma once
#include "Transform.hpp"

class Player
{
public:
    Player();

    const unsigned m_id;

    Transform m_transform;
};