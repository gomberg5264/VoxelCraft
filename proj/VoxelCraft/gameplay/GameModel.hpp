#pragma once
#include "common/Application.hpp"
#include "common/Player.hpp"
#include "common/Chunk.hpp"

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

    ChunkContainer m_chunks;
    std::vector<Player> m_players;
};