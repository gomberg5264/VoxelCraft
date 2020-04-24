#pragma once

class GameModel;
class Camera;

class GameView
{
public:
    void Update(const GameModel& model, const Camera& camera) const;
};