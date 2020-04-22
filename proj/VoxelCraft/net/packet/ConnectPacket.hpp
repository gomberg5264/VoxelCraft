#pragma once
#include "net/packet/Packet.hpp"

/**
 * These packets concern connection
 * operations such as joining, leaving, kicking
 */

class JoinRequestPacket : public DataPacket
{
public: 
    JoinRequestPacket(const char* name) 
        : DataPacket(PacketType::JoinRequest) 
        , m_name(name)
    {}

    inline const std::string& GetName() const { return m_name; }

    virtual void OnBuild(Packet& packet) override final { packet << m_name; }
    virtual void OnExtract(Packet&& packet) override final { packet >> m_name; }

private:
    std::string m_name;
};

class JoinReturnPacket : public DataPacket
{
public:
    enum class Status 
    {
        Accepted = 0,
        Full
    };

    JoinReturnPacket(Status status)
        : DataPacket(PacketType::JoinReturn)
        , m_status(status)
    {}

    inline Status GetStatus() { return m_status; }
    virtual void OnBuild(Packet& packet) override final { packet << static_cast<int>(m_status); }
    virtual void OnExtract(Packet&& packet) override final { int i; packet >> i; m_status = static_cast<Status>(i); }

private:
    Status m_status;
};

class JoinReturnPacket : public DataPacket
{
public:
    enum class Status
    {
        Accepted = 0,
        Full
    };

    JoinReturnPacket(Status status)
        : DataPacket(PacketType::JoinReturn)
        , m_status(status)
    {}

    inline Status GetStatus() { return m_status; }
    virtual void OnBuild(Packet& packet) override final { packet << static_cast<int>(m_status); }
    virtual void OnExtract(Packet&& packet) override final { int i; packet >> i; m_status = static_cast<Status>(i); }

private:
    Status m_status;
};

class ShutdownPacket : public DataPacket
{
public:
    ShutdownPacket() : DataPacket(PacketType::Shutdown) {}
};