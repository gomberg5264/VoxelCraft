#pragma once
#include <unordered_map>

/**
 * Used to index in the texture atlas
 * Needed by both texture atlas and block
 */
enum BlockFace
{
    Top,
    Bottom,
    Left,
    Right,
    Back,
    Front,

    Count
};

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone,

    Count
};

struct BlockData
{
    bool isSolid{ true };

    // Index is from top left. x, y.
    unsigned texture[BlockFace::Count];

    /////////////////////////
    // Some helper functions
    /////////////////////////
    inline void SetSideTexture(unsigned texIndex)
    {
        texture[BlockFace::Left] =
        texture[BlockFace::Back] =
        texture[BlockFace::Front] =
        texture[BlockFace::Right] = texIndex;
    }
    
    inline void SetTexture(unsigned texIndex)
    {
        for (int i = 0; i < BlockFace::Count; i++) texture[i] = texIndex;
    }

    inline void SetSideUpBottomTexture(unsigned side, unsigned up, unsigned bottom)
    {
        SetSideTexture(side);
        texture[BlockFace::Top] = up;
        texture[BlockFace::Bottom] = bottom;
    }
};

class BlockLibrary
{
public:
    static void AddBlockType(const BlockType& type, const BlockData& block)
    {
        assert(m_data.count(type) == 0);

        m_data[type] = block;
    }

    static const BlockData& GetBlockData(BlockType type)
    {
        return m_data.at(type);
    }

    static const std::unordered_map<BlockType, BlockData>& GetData()
    {
        return m_data;
    }

private:
    static std::unordered_map<BlockType, BlockData> m_data;
};