#pragma once
#include "net/packet/Packet.hpp"
#include "common/Player.hpp"

/**
 * These packets concern connection
 * operations such as joining, leaving, kicking
 */

class ConnectPacket : public PacketData
{
public: 
    ConnectPacket() : PacketData(PacketType::Connect) {};
    ConnectPacket(const char* name)
        : PacketData(PacketType::Connect)
        , m_name(name)
    {}

    inline const std::string& GetName() const { return m_name; }

    virtual void OnBuild(Packet& packet) const override final { packet << m_name; }
    virtual void OnExtract(Packet&& packet) override final { packet >> m_name; }

private:
    std::string m_name;
};

class ConnectResponsePacket : public PacketData
{
public:
    enum class Status 
    {
        Accepted = 0,
        Full
    };

    ConnectResponsePacket() : PacketData(PacketType::ConnectResponse) {};
    ConnectResponsePacket(Status status, const std::vector<Player>& players)
        : PacketData(PacketType::ConnectResponse)
        , m_status(status)
        , playerCount(players.size())
        , players(players)
    {}

    unsigned playerCount;
    std::vector<Player> players;

    inline Status GetStatus() { return m_status; }
    virtual void OnBuild(Packet& packet) const override final 
    { 
        packet << static_cast<int>(m_status); 
        packet << playerCount;

        for (int i = 0; i < playerCount; i++)
            packet << players[i];
    }
    virtual void OnExtract(Packet&& packet) override final 
    { 
        int i; 
        packet >> i; 
        m_status = static_cast<Status>(i); 
        packet >> playerCount;

        for (int i = 0; i < playerCount; i++)
        {
            players.emplace_back();
            packet >> players[i];
        }
    }

private:
    Status m_status;
};

class DisconnectPacket : public PacketData
{
public:
    DisconnectPacket() : PacketData(PacketType::Disconnect) {};
};

class ShutdownPacket : public PacketData
{
public:
    ShutdownPacket() : PacketData(PacketType::Shutdown) {}
};