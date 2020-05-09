#include "Common/ClientLayer.h"
#include "Common/ServerLayer.h"

#include <cereal/archives/json.hpp>
#include "Net/Packet.h"

#include <Shinobu/Common.h>
#include <enet/enet.h>

#include <fstream>

std::unique_ptr<sh::Application> sh::CreateApplication()
{
    enet_initialize();

    {
        //std::stringstream stream;
        //{     
        //    std::unique_ptr<Packet> response = std::make_unique<JoinResponse>();
        //    static_cast<JoinResponse*>(response.get())->userID = 199;

        //    cereal::PortableBinaryOutputArchive out(stream);
        //    out(response);
        //}

        //{
        //    cereal::PortableBinaryInputArchive in(stream);

        //    std::unique_ptr<Packet> packet;
        //    in(packet);
        //    std::cout << static_cast<JoinResponse*>(packet.get())->userID;
        //}

        //{
        //    auto response = std::make_unique<JoinResponse>();
        //    response->userID = 199;

        //    auto binary = PacketToBinary(response);
        //    auto point = PacketFromBinary(binary);

        //    std::cout << static_cast<JoinResponse*>(point.get())->userID;
        //}

        //std::cin.get();
    }

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