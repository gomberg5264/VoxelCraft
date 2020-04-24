#pragma once
#include "common/Application.hpp"
#include "common/Player.hpp"

#include <vector>

/**
 * All the data that should be shared among the machine is contained in the
 * GameModel class. 
 */
class GameModel
{
public:
    GameModel();
	void Update();

    void LoadChunk(const glm::vec3& pos);
    void RemoveChunk(const glm::vec3& pos);

	std::vector<Player> m_players;
};