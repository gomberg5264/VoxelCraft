#pragma once

/**
 * I wrap the packet class already for if 
 * I want to encrypt/compress the data
 */
class Packet : public sf::Packet
{
};

/**
 * PacketData will extract the udp packet data 
 * The extraction happens in the server class
 */
struct BasePacket
{
public:

    enum class Type
    {
        Handshake
    };

    enum class Status
    {
        Valid,
        Unrelated,
        Failed
    };

    const int projectID = 105;
    Type type;

    Packet Build();
    Status Extract(Packet& packet);

    virtual void OnBuild(Packet& packet) {};
    virtual void OnExtract(Packet& packet) {};
};