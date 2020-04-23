#pragma once
#include "net/packet/Packet.hpp"

class MessagePacket : public PacketData
{
public:
    MessagePacket() : PacketData(PacketType::Message) {}
    MessagePacket(const std::string& message) 
        : PacketData(PacketType::Message) 
        , message(message) {}

    std::string message;

private:
    virtual void OnBuild(Packet& packet) const { packet << message; };
    virtual void OnExtract(Packet&& packet) { packet >> message; };
};