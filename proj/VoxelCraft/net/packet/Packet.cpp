#include "vcpch.hpp"
#include "net/packet/Packet.hpp"

Packet DataPacket::Build()
{
    Packet packet;
    packet << projectID << static_cast<int>(type);
    OnBuild(packet);
    return packet;
}

void DataPacket::Extract(Packet&& packet)
{
    OnExtract(std::move(packet));
}

PacketType VerifyPacket(Packet& packet)
{
    PacketType status;
    int id;
    int type;
    if (packet >> id >> type)
    {
        // Check if ID is right version
        if (id == DataPacket::projectID)
        {
            return static_cast<PacketType>(type);
        }
        else
        {
            return PacketType::Unrelated;
        }
    }
    else
    {
        return PacketType::Unrelated;
    }
}