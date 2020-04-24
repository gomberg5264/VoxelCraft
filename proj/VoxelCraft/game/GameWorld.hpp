#pragma once
#include "common/Application.hpp"
#include "common/Player.hpp"

#include <vector>

/**
 * The game world represents the game we are making
 */

class GameWorld
{
public:
	void OnUpdate();

	std::vector<Player> m_players;
};

class GameWorldInput
{
public:
    void OnUpdate();

};

class GameWorldVisual
{
public:
    void OnUpdate();

};