#pragma once

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

enum class PacketType
{
    JoinPacket
};

struct Packet
{
public:
    virtual PacketType GetType() = 0;

    unsigned packetID = 1;

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(packetID);
    }
};

struct JoinPacket : public Packet
{
public:
    virtual PacketType GetType() { return PacketType::JoinPacket; }

    unsigned userCount;

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::base_class<Packet>(this));
        ar(userCount);
    }
};

CEREAL_REGISTER_TYPE(JoinPacket)