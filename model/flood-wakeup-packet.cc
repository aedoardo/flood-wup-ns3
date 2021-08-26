#include "flood-wakeup-packet.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/mac16-address.h"
#include "ns3/address-utils.h"

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("FloodWUPPacketHeader");
    NS_OBJECT_ENSURE_REGISTERED(FloodWUPPacketHeader);


    TypeId FloodWUPPacketHeader::GetTypeId(void) {
        static TypeId tid = 
            TypeId("ns3:FloodWUPPacketHeader")
            .SetParent<Chunk>()
            .SetGroupName("Wur");

        return tid;
    }

    FloodWUPPacketHeader::FloodWUPPacketHeader() {

    }

    FloodWUPPacketHeader::~FloodWUPPacketHeader() {

    }

    TypeId FloodWUPPacketHeader::GetInstanceTypeId() const {
        return GetTypeId();
    }

    uint32_t FloodWUPPacketHeader::GetSerializedSize() const {
        return sizeof(Mac16Address) * 1;
    }

    void FloodWUPPacketHeader::Serialize(Buffer::Iterator start) const {
        NS_LOG_DEBUG("Serializing wakeup packet.");
        Buffer::Iterator i = start;
        WriteTo(i, wakeUpSequence);
    }

    uint32_t FloodWUPPacketHeader::Deserialize(Buffer::Iterator start) {
        Buffer::Iterator i = start;
        ReadFrom(i, wakeUpSequence);

        NS_LOG_DEBUG("Length remaining: " + i.GetDistanceFrom(start));
        return i.GetDistanceFrom(start);
    }

    void FloodWUPPacketHeader::Print(std::ostream &os) const {
	    os << wakeUpSequence;
    }

}