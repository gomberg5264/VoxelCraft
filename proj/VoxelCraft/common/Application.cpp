#include "vcpch.hpp"

#include "common/Application.hpp"

#include "net/ClientLayer.hpp"

Timer Core::time;
constexpr size_t maxLayers = 10;

void Application::AddLayer(std::unique_ptr<Layer>&& layer)
{
    m_layers.push_back(std::move(layer));
    if (m_layers.size() > maxLayers) throw "Too many layers";

    AddSubscriber(*m_layers.back());
    m_layers.back()->Init(*this);
}

void Application::Run() noexcept
{
    m_layers.reserve(maxLayers);

    Core::time.Reset();
    CreateApplication(m_layers);

    // Initialize layers
    for (auto& layer : m_layers) { AddSubscriber(*layer); }
    for (auto& layer : m_layers) { layer->Init(*this); }
    
    if (m_layers.size() > maxLayers) throw "Too many layers";

    bool shouldExit = false;
    while (!shouldExit)
    {
        Core::time.Update();

        for (int i = 0; i < m_layers.size(); i++)
        {
            auto& layer = m_layers[i];

            layer->Update();
            if (layer->ShouldExit()) shouldExit = true;
        }

        m_layers.erase(std::remove_if(m_layers.begin(), m_layers.end(), [&](const std::unique_ptr<Layer>& layer)
            {
                if (layer->ShouldPop())
                {
                    layer->Deinit();
                    RemoveSubscriber(*layer);
                    return true;
                }
                return false;
            }), m_layers.end());
    }

    for (auto& layer : m_layers)
        layer->Deinit();
    m_layers.clear();
}

void Layer::Init(Application& app)
{
    m_app = &app;
    // Application registers subscribers to itself since a layer could 
    // emit an event during initialization
    //m_app->Subscribe(*this);
    OnInit();
}

void Layer::Deinit()
{
    OnDeinit();
}

void Layer::Update()
{
    OnUpdate();
}