#pragma once

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

#include <sstream>
#include <memory>

enum class PacketType
{
    JoinResponse
};

struct Packet
{
public:
    virtual PacketType GetType() const = 0;

    unsigned packetID = 1;

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(packetID);
    }
};

std::stringstream PacketToBinary(Packet& packet);
std::unique_ptr<Packet> PacketFromBinary(std::stringstream& binary);

struct JoinResponse : public Packet
{
public:
    virtual PacketType GetType() const { return PacketType::JoinResponse; }

    unsigned userID;

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::base_class<Packet>(this));
        ar(userID);
    }
};

CEREAL_REGISTER_TYPE(JoinResponse)