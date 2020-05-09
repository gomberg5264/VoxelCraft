#include "Net/Packet.h"

std::stringstream PacketToBinary(Packet& packet)
{
    std::stringstream stream;
    {
        cereal::PortableBinaryOutputArchive out(stream);
        out << packet;
    }

    return stream;
}

std::unique_ptr<Packet> PacketFromBinary(std::stringstream& binary)
{
    std::unique_ptr<Packet> packet;

    {
        cereal::PortableBinaryInputArchive in(binary);
        in >> packet;
    }

    return std::move(packet);
}