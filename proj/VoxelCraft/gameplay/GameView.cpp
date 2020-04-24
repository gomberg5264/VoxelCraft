#include "vcpch.hpp"
#include "gameplay/GameView.hpp"
#include "gameplay/GameModel.hpp"

#include "common/Application.hpp"
#include "client/gl/PlayerRenderer.hpp"

void GameView::Update(const GameModel& model, const Camera& camera) const
{
    constexpr float dayDur = 1.f / 8.0f;
    const float time = Core::time.Total() * dayDur;

    // TODO: Not have this generate every frame
    //std::vector<PlayerMesh> meshses;
    for (const auto& player : model.m_players)
        PlayerMesh(player.m_transform).Draw(camera);
        //meshses.emplace_back(PlayerMesh(player.m_transform));
}
