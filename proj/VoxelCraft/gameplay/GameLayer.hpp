#pragma once
#include "common/Application.hpp"
#include "common/event/WindowEvent.hpp"
#include "common/event/InputEvent.hpp"
#include "common/event/NetEvent.hpp"

#include "gameplay/GameModel.hpp"
#include "gameplay/GameView.hpp"

#include "client/gl/Window.hpp"
#include "client/Camera.hpp"
#include "client/ChunkStrategy.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <iostream>
#include <thread>

/**
 * Handles the data that goes to the GameModel
 */
class GameClientLayer : public Layer
{
private:
    virtual void OnInit() override final;
    virtual void OnNotify(Event& event) override final;
    virtual void OnUpdate() override final;

    void InitGame();

    Window m_window;
    std::unique_ptr<FreelookCamera> m_camera;

    ChunkStrategy m_strat;
    GameModel m_model;
    GameView m_view;

    std::thread m_console;
};


class GameServerLayer : public Layer
{
public:
    virtual void OnInit() override final;
    virtual void OnNotify(Event& event) override final;
    virtual void OnUpdate() override final;

    GameModel m_model;
};