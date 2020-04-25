#pragma once
#include "common/Chunk.hpp"

#include "client/gl/Texture.hpp"
#include "client/gl/Window.hpp"
#include "client/gl/ChunkRenderer.hpp"
#include "client/gl/PlayerRenderer.hpp"
#include "client/gl/SkyRenderer.hpp"

#include <glm/gtx/hash.hpp>
#include <unordered_map>

class GameModel;
class Camera;

class GameView
{
public:
    void AddChunk(Chunk& chunk);
    void RemoveChunk(Chunk& chunk);
    
    void Draw(const GameModel& model, const Camera& camera);

private:
    std::unordered_map<glm::ivec3, ChunkMesh> m_chunkMeshes;
    ChunkRenderer m_chunkRenderer;
    SkyRenderer m_skyRenderer;
};