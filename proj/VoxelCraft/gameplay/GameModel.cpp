#include "vcpch.hpp"
#include "gameplay/GameModel.hpp"
#include "common/BlockData.hpp"

void RegisterBlockTypes(unsigned atlasX, unsigned atlasY)
{
    // Register block types
    // This function converts an x and y coordinate to an index
    auto T = [atlasX, atlasY](unsigned x, unsigned y)
    {
        return x + (atlasY - 1 - y) * atlasX;
    };

    // Register block data
    // ---
    // TODO Move this to a function or to be implemented in scripting
    {
        BlockData block;
        block.isSolid = false;
        BlockDataFactory::GetInstance().AddBlockData(BlockType::Air, block);
    }
    {
        BlockData block;
        block.SetSideUpBottomTexture(
            T(1, 1),
            T(0, 0),
            T(0, 1));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Grass, block);
    }
    {
        BlockData block;
        block.SetTexture(T(0, 1));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Dirt, block);
    }
    {
        BlockData block;
        block.SetTexture(T(1, 0));

        BlockDataFactory::GetInstance().AddBlockData(BlockType::Stone, block);
    }
}

GameModel::GameModel()
{
    // Set up block types data
    const auto atlasX = 2;
    const auto atlasY = 2;
    RegisterBlockTypes(atlasX, atlasY);

    m_players.emplace_back();
    m_players.emplace_back();
}

void GameModel::Update()
{
    const float et = Core::time.Total();
    const float dt = Core::time.Elapsed();

    for (int i = 0; i < m_players.size(); i++)
    {
        auto& player = m_players[i];

        player.m_transform.m_pos = glm::vec3(i * 5, 20, 20.f);
        player.m_transform.m_scale = glm::vec3(glm::sin(et), glm::cos(et), glm::sin(glm::cos(et))) * 10.f;
        player.m_transform.m_euler = player.m_transform.m_euler + glm::vec3(dt, dt * 3.f, dt * 1.5f);
    }
}