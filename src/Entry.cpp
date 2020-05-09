#include "Common/ClientLayer.h"
#include "Common/ServerLayer.h"
#include "Net/Packet.h"

#include <Shinobu/Common.h>
#include <enet/enet.h>

std::unique_ptr<sh::Application> sh::CreateApplication()
{
    enet_initialize();

    int i;
    std::cout << "1 server, 2 client\n";
    std::cin >> i;
    std::cin.ignore();

    WindowProps props;
    props.title = "VoxelCraft";
    props.width = 1;
    props.height= 1;
    auto app = std::make_unique<sh::Application>(props);

    if (i == 1)
    {
        app->GetLayerStack().PushLayer(new ServerLayer);
    }
    else
    {
        app->GetLayerStack().PushLayer(new ClientLayer);
    }

    return app;
}