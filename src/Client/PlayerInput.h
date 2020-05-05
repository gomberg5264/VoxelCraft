#pragma once
#include "Common/Player.h"

#include <Shinobu/Core/Timestep.h>
#include <Shinobu/Event/Event.h>

#include <functional>

class PlayerInput
{
public:
    unsigned playerID;

    void OnUpdate(sh::Timestep ts);
    void OnEvent(sh::Event& event);

    using CommandCB = std::function<void(Command&)>;

    CommandCB callback;
};