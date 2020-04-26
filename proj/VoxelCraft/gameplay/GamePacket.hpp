#pragma once
#include "net/packet/Packet.hpp"

enum class GamePacketType : unsigned
{
    Initial, Join, 
    
    Move, Message,
};

inline sf::Packet& operator<<(sf::Packet& p, const GamePacketType& t)
{
    p << static_cast<unsigned>(t);
    return p;
}
inline sf::Packet& operator>>(sf::Packet& p, GamePacketType& t)
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
    GameInitialPacket() : GamePacket(GamePacketType::Initial) {}
    
    unsigned id;
    std::vector<Player> players;

    virtual void OnBuild(Packet& packet) const { GamePacket::OnBuild(packet); packet << id << players; };
    // NOTE: We don't call GamePacket::OnExtract since we extract manually to check the type
    virtual void OnExtract(Packet& packet) { packet >> id >> players; };
};

struct GameJoinPacket : public GamePacket
{
    GameJoinPacket() : GamePacket(GamePacketType::Join) {}
    GameJoinPacket(const Player& player) 
        : GamePacket(GamePacketType::Join)
        , player(player) {}

    Player player;

    virtual void OnBuild(Packet& packet) const { GamePacket::OnBuild(packet); packet << player; };
    // NOTE: We don't call GamePacket::OnExtract since we extract manually to check the type
    virtual void OnExtract(Packet& packet) { packet >> player; };
};

// TODO: Make this work on entities
struct GameMovePacket : public GamePacket
{
    GameMovePacket() : GamePacket(GamePacketType::Move) {}

    // TODO: We should probably send a move instead of the player since 
    // we should not be modifying the player on client side
    GameMovePacket(const Player& player)
        : GamePacket(GamePacketType::Move)
        , player(player) {}

    Player player;

    virtual void OnBuild(Packet& packet) const { GamePacket::OnBuild(packet); packet << player; };
    // NOTE: We don't call GamePacket::OnExtract since we extract manually to check the type
    virtual void OnExtract(Packet& packet) { packet >> player; };
};