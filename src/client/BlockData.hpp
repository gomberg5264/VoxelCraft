#pragma once

enum class BlockType
{
    Air,
    Grass,
    Dirt,
    Stone,

    Count
};

/**
 * Used to index in the texture atlas
 * Needed by both texture atlas and block
 */

enum BlockFace
{
    Back,
    Front,
    Left,
    Right,
    Bottom,
    Top,

    Count
};

struct BlockData
{
    bool isSolid{ true };

    // Index is from top left. x, y.
    unsigned texture[BlockFace::Count];

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

    VBO CreateVBO(glm::vec3 pos) const
    {
        VBO vbo{ 1 };
        VBO::Vertex ver;
        ver.pos[0] = pos.x;
        ver.pos[1] = pos.y;
        ver.pos[2] = pos.z;
        for (int i = 0; i < BlockFace::Count; i++) ver.texIndex[i] = texture[i];

        vbo.WriteBack(ver);
        return vbo;
    }
};

class BlockDataFactory
{
public:
    using Value = BlockData;

    inline void AddBlockData(const BlockType& type, const BlockData& block)
    {
        assert(m_data.count(type) == 0);

        m_data[type] = block;
    }

    inline const Value& GetBlockData(BlockType type) const
    {
        return m_data.at(type);
    }

    inline const std::unordered_map<BlockType, Value>& GetData() const
    {
        return m_data;
    }

private:
    std::unordered_map<BlockType, Value> m_data;
};