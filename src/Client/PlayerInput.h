#pragma once
#include "Common/Player.h"

#include <Shinobu/Core/Timestep.h>
#include <Shinobu/Event/Event.h>

class PlayerInput
{
public:
    // TODO: Create some id system since we have to serialize which entity we referring to
    Player* player;

    void OnUpdate(sh::Timestep ts);
    void OnEvent(sh::Event& event);
};