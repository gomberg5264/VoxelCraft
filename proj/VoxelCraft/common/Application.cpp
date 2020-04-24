#include "vcpch.hpp"

#include "common/Application.hpp"

Timer Core::time;

void Application::Run() noexcept
{



    Core::time.Reset();
    CreateApplication(m_layers);

    // Initialize layers
    
    for (auto& layer : m_layers) { AddSubscriber(*layer); }
    for (auto& layer : m_layers) { layer->Init(*this); }

    // TODO: Add uninitialize for layers
    bool shouldExit = false;
    while (!shouldExit)
    {
        Core::time.Update();

        // Initialize layers
        for (auto& layer : m_layers)
        {
            layer->Update();
            if (layer->ShouldExit()) shouldExit = true;
        }
    }
}

void Layer::Init(Application& app)
{
    m_app = &app;
    // Application registers subscribers to itself since a layer could 
    // emit an event during initialization
    //m_app->Subscribe(*this);
    OnInit();
}

void Layer::Update()
{
    OnUpdate();
}