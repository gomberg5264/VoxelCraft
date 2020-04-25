#pragma once
#include "net/packet/Packet.hpp"

enum class GamePacketType : unsigned
{
    Message = 1, 
};

inline Packet& operator<<(Packet& p, const GamePacketType& t)
{
    p << static_cast<unsigned>(t);
    return p;
}
inline Packet& operator>>(Packet& p, GamePacketType& t)
{
    unsigned i;
    p >> i;
    t = static_cast<GamePacketType>(i);
    return p;
}

class GamePacket : public GameplayPacket
{
public:
    GamePacket(GamePacketType type) : gameType(type) {}

    virtual void OnBuild(Packet& packet) const { packet << gameType; };
    virtual void OnExtract(Packet& packet) { packet >> gameType; };

    GamePacketType gameType;
};

struct GameMessagePacket : public GamePacket
{
    GameMessagePacket(): GamePacket(GamePacketType::Message) {}
    GameMessagePacket(const char* message) 
        : GamePacket(GamePacketType::Message)
        , message(message) {}

    virtual void OnBuild(Packet& packet) const { GamePacket::OnBuild(packet); packet << message; };
    // NOTE: We don't call GamePacket::OnExtract since we extract manually to check the type
    virtual void OnExtract(Packet& packet) { packet >> message; };

    std::string message;
};

//class MessagePacket : public PacketData
//{
//public:
//    MessagePacket() : PacketData(PacketType::Message) {}
//    MessagePacket(const std::string& message) 
//        : PacketData(PacketType::Message) 
//        , message(message) {}
//
//    std::string message;
//
//private:
//    virtual void OnBuild(Packet& packet) const { packet << message; };
//    virtual void OnExtract(Packet& packet) { packet >> message; };
//};