//#include "Common/GameLayer.h"
//#include "Net/NetworkLayer.h"

#include <Shinobu/Common.h>
#include <enet/enet.h>

#include "Net/Packet.h"

std::unique_ptr<sh::Application> sh::CreateApplication()
{
    enet_initialize();

    {
        // Create archive
        std::stringstream stream;
        cereal::PortableBinaryOutputArchive out(stream);

        // Serialize
        auto join = std::make_unique<JoinPacket>();
        join->userCount = 20;


        out << join;
        join->userCount = 0;

        cereal::PortableBinaryInputArchive in(stream);
        in >> join;

        std::cout << join->userCount << '\n';
        std::cin.get();

    }



    //int i;
    //std::cout << "0 server, 1 client\n";
    //std::cin >> i;
    //std::cin.ignore();

    WindowProps props;
    props.title = "VoxelCraft";
    //props.width = 1;
    //props.height= 1;
    auto app = std::make_unique<sh::Application>(props);
    //app->GetLayerStack().PushLayer(new GameLayer);


    //if (i == 0)
    //{
    //    app->GetLayerStack().PushLayer(new ServerLayer);
    //}
    //else
    //{
    //    app->GetLayerStack().PushLayer(new ClientLayer);
    //}

    return app;
}