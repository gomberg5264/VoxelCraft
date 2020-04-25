#include "vcpch.hpp"
#include "gameplay/GameView.hpp"
#include "gameplay/GameModel.hpp"

#include "common/Application.hpp"
#include "client/gl/PlayerRenderer.hpp"

void GameView::AddChunk(Chunk& chunk)
{
    if (m_chunkMeshes.count(chunk.GetPos()) != 0)
        return;

    // TODO: check if we have to generate the mesh
    m_chunkMeshes.emplace(chunk.GetPos(), ChunkMesh());
    auto& val = m_chunkMeshes.at(chunk.GetPos());
    val.Generate(chunk);

    m_chunkRenderer.RegisterEBOToVAO(val.m_vao);
}

void GameView::RemoveChunk(Chunk& chunk)
{
    if (m_chunkMeshes.count(chunk.GetPos()) == 0)
        return;

    m_chunkMeshes.erase(chunk.GetPos());
}

void GameView::Draw(const GameModel& model, const Camera& camera)
{
    constexpr float dayDur = 1.f / 8.0f;
    const float time = Core::time.Total() * dayDur;

    // -- Update chunk meshes
    {
        for (const auto& chunk : model.m_chunks.GetModifiedChunks())
        {
            // We only save chunks with a mesh, an air chunk could now have a mesh
            if (m_chunkMeshes.count(chunk.get().GetPos()) == 0)
                m_chunkMeshes.insert(std::make_pair(chunk.get().GetPos(), ChunkMesh()));
            
            if (chunk.get().m_isAir)
                m_chunkMeshes.erase(chunk.get().GetPos());
            else
                m_chunkMeshes.at(chunk.get().GetPos()).Generate(chunk);
        }
    }

    // -- Make draw requests to renderers -- 
    {
        for (const auto& player : model.m_players)
            PlayerMesh(player.m_transform).Draw(camera);

        // TODO: Render only on certain condition
        // TODO: Do culling
        for (const auto& chunk : m_chunkMeshes)
        {
            if (chunk.second.m_elemCount != 0)
            {
                m_chunkRenderer.Render(chunk.second);
            }
        }
    }

    // -- Set up renderers state --
    {
        // Update sky renderer by passing current time
        m_skyRenderer.SetTime(std::fmod(time, 1.f));

        // Set up ligting for renderers
        m_chunkRenderer.SetSkyIntensity(m_skyRenderer.GetSkyAmbient());
        m_chunkRenderer.SetSkyLightDirection(m_skyRenderer.GetLightDir());
        m_chunkRenderer.SetSkyLightColor(m_skyRenderer.GetColor());
        m_chunkRenderer.SetDiffuseIntensity(m_skyRenderer.GetIntensity());

        // Set up camera matrices
        m_skyRenderer.SetCameraRotateProject(camera.GetProjection() * camera.GetRotation());
        m_chunkRenderer.SetVP(camera.GetProjection() * camera.GetView());
    }

    // -- Render contents to the screen --
    {
        m_chunkRenderer.Display();
        m_skyRenderer.Display();
    }
}