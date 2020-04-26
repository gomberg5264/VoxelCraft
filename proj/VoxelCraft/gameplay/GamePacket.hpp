#pragma once
#include "net/packet/Packet.hpp"

enum class GamePacketType : unsigned
{
    Join, 
    
    Message, 
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

// This packet will be send when a player has joined
// It contains initial world state
struct GameInitialPacket : public GamePacket
{
    GameInitialPacket() : GamePacket(GamePacketType::Join) {}

    std::vector<Player> players;

    virtual void OnBuild(Packet& packet) const { GamePacket::OnBuild(packet); packet << players; };
    // NOTE: We don't call GamePacket::OnExtract since we extract manually to check the type
    virtual void OnExtract(Packet& packet) { packet >> players; };
};
