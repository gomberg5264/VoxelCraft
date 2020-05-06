#pragma once
#include "Common/Player.h"

#include <Shinobu/Core/Timestep.h>
#include <Shinobu/Event/Event.h>

#include <functional>
#include <memory>

class PlayerInput
{
public:
    PlayerInput(unsigned playerID)
        : playerID(playerID) {}

    const unsigned playerID;

    void OnUpdate(sh::Timestep ts);
    void OnEvent(sh::Event& event);

    using CommandCB = std::function<void(std::unique_ptr<Command>&&)>;

    CommandCB callback;
};