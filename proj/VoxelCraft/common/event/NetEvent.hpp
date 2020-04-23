#pragma once
#include "common/event/Event.hpp"

#include "net/packet/Packet.hpp"

class NetPacketEvent : public Event
{
public:
    NetPacketEvent(PacketData& packet) : m_packet(packet) {}
    inline PacketData& GetPacketData() { return m_packet; }

    EVENT_CLASS_CATEGORY(EventCategory::Net)

private:
    PacketData& m_packet;
};

class NetSendPacketEvent : public NetPacketEvent
{
public:
    using NetPacketEvent::NetPacketEvent;    
    EVENT_CLASS_TYPE(EventType::NetSendPacket)
};

class NetReceivePacketEvent : public NetPacketEvent
{
public:
    using NetPacketEvent::NetPacketEvent;
    EVENT_CLASS_TYPE(EventType::NetReceivePacket)
};