#pragma once
#include "net/packet/Packet.hpp"

/**
 * These packets concern connection
 * operations such as joining, leaving, kicking
 */

class JoinRequestPacket : public PacketData
{
public: 
    JoinRequestPacket() : PacketData(PacketType::JoinRequest) {};
    JoinRequestPacket(const char* name)
        : PacketData(PacketType::JoinRequest) 
        , m_name(name)
    {}

    inline const std::string& GetName() const { return m_name; }

    virtual void OnBuild(Packet& packet) const override final { packet << m_name; }
    virtual void OnExtract(Packet&& packet) override final { packet >> m_name; }

private:
    std::string m_name;
};

class JoinReturnPacket : public PacketData
{
public:
    enum class Status 
    {
        Accepted = 0,
        Full
    };

    JoinReturnPacket() : PacketData(PacketType::JoinReturn) {};
    JoinReturnPacket(Status status)
        : PacketData(PacketType::JoinReturn)
        , m_status(status)
    {}

    inline Status GetStatus() { return m_status; }
    virtual void OnBuild(Packet& packet) const override final { packet << static_cast<int>(m_status); }
    virtual void OnExtract(Packet&& packet) override final { int i; packet >> i; m_status = static_cast<Status>(i); }

private:
    Status m_status;
};

class ShutdownPacket : public PacketData
{
public:
    ShutdownPacket() : PacketData(PacketType::Shutdown) {}
};