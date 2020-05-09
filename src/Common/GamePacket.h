#pragma once
#include "Common/Command.h"
#include "Net/Packet.h"

#include "Common/Entity/EntityCommand.h"
#include <memory>

struct JoinResponsePacket : public Packet
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
CEREAL_REGISTER_TYPE(JoinResponsePacket)

struct EntityCommandPacket : public Packet
{
public:
    EntityCommandPacket() = default;

    EntityCommandPacket(const EntityCommandPacket& rhs)
    {
        // TODO: IMPLEMENT THIS COPY CONSTRUCTOR 
        // so that you can copy the command to other machines


        //std::unique_ptr<Packet> data(new T::element_type(*packet));

        //command = std::make_unique<EntityCommand>(new )
        //command = std::make_unique(new rhs.command::element_type(*rhs.command));
        //command = std::make_unique<rhs.c
    }

    virtual PacketType GetType() const { return PacketType::JoinResponse; }

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::base_class<Packet>(this));
        ar(command);
    }

public:
    std::unique_ptr<EntityCommand> command;
};
CEREAL_REGISTER_TYPE(EntityCommandPacket)