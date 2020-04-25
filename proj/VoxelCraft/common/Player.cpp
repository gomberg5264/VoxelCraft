#include "vcpch.hpp"
#include "Player.hpp"

static unsigned int unique = 0;
Player::Player()
    : m_id(unique++)
{
}
