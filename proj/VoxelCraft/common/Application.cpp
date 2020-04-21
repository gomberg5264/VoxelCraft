#include "vcpch.hpp"

#include "common/Application.hpp"
#include "common/BlockData.hpp"

Timer Core::time;

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

Application::Application()
{
    // Set up block types
    const auto atlasX = 2;
    const auto atlasY = 2;
    RegisterBlockTypes(atlasX, atlasY);
}

void Application::Run() noexcept
{
    auto layer = CreateApplication();

    Core::time.Reset();

    layer->OnInit();
    while (layer->m_run)
    {
        Core::time.Update();
        layer->OnUpdate();
    }
}
