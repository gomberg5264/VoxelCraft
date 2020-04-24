#include "vcpch.hpp"
#include "gameplay/GameView.hpp"
#include "gameplay/GameModel.hpp"

#include "common/Application.hpp"
#include "client/gl/PlayerRenderer.hpp"

void GameView::AddChunkCB(Chunk& chunk)
{
    // TODO: check if we have to generate the mesh
    auto& val = m_chunks[chunk.GetPos()];
    val.chunk = &chunk;
    val.mesh.Generate(chunk);
}

void GameView::RemoveChunkCB(Chunk& chunk)
{
    m_chunks.erase(chunk.GetPos());
}

void GameView::ModifyChunkCB(Chunk& chunk)
{
    // TODO: check if mesh needs to be generated
    m_chunks.at(chunk.GetPos()).mesh.Generate(chunk);
}

void GameView::Draw(const GameModel& model, const Camera& camera)
{
    constexpr float dayDur = 1.f / 8.0f;
    const float time = Core::time.Total() * dayDur;

    // TODO: Not have this generate every frame
    //std::vector<PlayerMesh> meshses;
        //meshses.emplace_back(PlayerMesh(player.m_transform));

    // RENDERING
    // ---------------------
    // Make draw requests to renderers
    //m_chunkManager->Render();

    // Set up renderers state
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

    // Render contents to the screen
    //m_window.Clear();

    for (const auto& player : model.m_players)
        PlayerMesh(player.m_transform).Draw(camera);

    // TODO: Render only on certain condition
    // TODO: Do culling
    for (const auto& chunk : m_chunks)
    {
        if (!chunk.second.chunk->m_isAir && chunk.second.mesh.m_elemCount != 0)
        {
            m_chunkRenderer.Render(chunk.second.mesh);
        }
    }

    m_chunkRenderer.Display();
    m_skyRenderer.Display();

    // TODO: Make a player renderer/entity renderer
    //m_playerMesh->Draw(camera);

    //m_window.Display();
}
