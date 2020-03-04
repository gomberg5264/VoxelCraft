#pragma once

enum BlockType
{
    Grass,
    Dirt,
    Stone,

    Count
};

struct BlockData
{
    BlockType type;
    Texture<unsigned> texture;
};

class BlockDataFactory
{
public:
    void AddBlockData(const BlockData& block)
    {
        assert(m_blocks.count(block.type) == 0);
        m_blocks[block.type] = block;
    }

    const BlockData& GetBlockData(BlockType type) const
    {
        return m_blocks.at(type);
    }

    BlockData CreateBlockData(BlockType type) const
    {
        return m_blocks.at(type);
    }

    const std::unordered_map<BlockType, BlockData>& GetData() const
    {
        return m_blocks;
    }

private:
    std::unordered_map<BlockType, BlockData> m_blocks;
};
