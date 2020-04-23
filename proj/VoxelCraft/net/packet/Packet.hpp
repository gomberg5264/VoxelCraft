#pragma once
#include <SFML/Network/Packet.hpp>

/**
 * I wrap the packet class already for if 
 * I want to encrypt/compress the data
 *
 * Packet format:
 * ID -> type -> custom
 */
class Packet : public sf::Packet
{
};

enum class PacketType
{
    Unrelated, // Unrelated packet, not interested in it 
    Connect, ConnectResponse, 
    Disconnect,

    Message,

    Shutdown,
};

/**
 * DataPacket is the interface for that data that we put in 
 * a packet. 
 */
struct PacketData
{
public:
    PacketData(PacketType type) : type(type) {}

    static const int projectID = 105;
    const PacketType type;
    
    Packet Build() const;
    void Extract(Packet&& packet);

private:
    virtual void OnBuild(Packet& packet) const {} ;
    virtual void OnExtract(Packet&& packet) {};
};

/**
 * Verifies if this packet is one that we are interested in and if it is
 * it returns the type of the packet so that we can extract the right one
 */
PacketType VerifyPacket(Packet& packet);

/**
 * Call this after VerifyPacket
 * Extracts the data from packet into T. This has to be split since
 * we need to already extract part of the data to check if we are
 * interested in the packet. 
 */
template <typename T>
T ExtractPacket(Packet& packet)
{
    static_assert(std::is_base_of<PacketData, T>::value, "T is not derived from PacketData");
    static_assert(std::is_default_constructible<T>::value, "T should be default constructable");

    T data;
    data.Extract(std::move(packet));
    return data;
}