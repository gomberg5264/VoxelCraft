#pragma once
#include "common/Chunk.hpp"

#include "client/gl/Texture.hpp"
#include "client/gl/Window.hpp"
#include "client/gl/ChunkRenderer.hpp"
#include "client/gl/PlayerRenderer.hpp"
#include "client/gl/SkyRenderer.hpp"

#include <unordered_map>

class GameModel;
class Camera;

class GameView
{
public:
    void AddChunkCB(Chunk& chunk);
    void RemoveChunkCB(Chunk& chunk);
    void ModifyChunkCB(Chunk& chunk);

    void Draw(const GameModel& model, const Camera& camera);

private:
    struct ChunkMapVal
    {
        Chunk* chunk;
        ChunkMesh mesh;
    };

    std::unordered_map<glm::ivec3, ChunkMapVal> m_chunks;
    ChunkRenderer m_chunkRenderer;
    SkyRenderer m_skyRenderer;
};