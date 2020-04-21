#include "vcpch.hpp"
#include "net/packet/Packet.hpp"

Packet BasePacket::Build()
{
    Packet packet;
    packet << projectID << static_cast<int>(type);
    OnBuild(packet);
    return packet;
}

BasePacket::Status BasePacket::Extract(Packet& packet)
{
    int id;
    int type;
    if (packet >> id >> type)
    {
        // Check if ID is right version
        if (id == projectID)
        {
            this->type = static_cast<Type>(type);
            OnExtract(packet);
            return Status::Valid;
        }
        else return Status::Unrelated;
    }
    else return Status::Unrelated;
}