#include "flood-wup-wakeup-packet.h"
#include "ns3/packet.h"

namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("FloodWUPWakeupPacket");
    NS_OBJECT_ENSURE_REGISTERED(FloodWUPWakeupPacket);

    TypeId FloodWUPWakeupPacket::GetTypeId(void) {
        static TypeId tid = TypeId("ns3:FloodWUPWakeupPacket")
            .SetParent<Header>()
            .SetGroupName("FloodWUP") // TODO: check.
            .AddConstructor<FloodWUPWakeupPacket>();
        
        return tid;
    }
    
    FloodWUPWakeupPacket::FloodWUPWakeupPacket() {
        NS_LOG_FUNCTION(this);

        m_sequenceNumber = 0;
        m_wakeUpSequence = NULL;

    }

    FloodWUPWakeupPacket::~FloodWUPWakeupPacket() {
        NS_LOG_FUNCTION(this);
    }

    uint32_t FloodWUPWakeupPacket::GetSerializedSize(void) const {
        return 0; // TODO: da impostare.
    }

    void FloodWUPWakeupPacket::Serialize(Buffer::Iterator start) const {
        Buffer::Iterator i = start;
        // TODO: struttura del pacchetto con solo header.
    }

    uint32_t FloodWUPWakeupPacket::Deserialize(Buffer::Iterator start) {
        Buffer::Iterator i = start;

        // Deserialize packet.
    }

    uint16_t FloodWUPWakeupPacket::GetLength() const {
        return m_length;
    }

    uint16_t FloodWUPWakeupPacket::GetSequenceNumber() const {
        return m_sequenceNumber;
    }

    Mac8Address FloodWUPWakeupPacket::GetWakeUpSequence() const {
        return m_wakeUpSequence;
    }

    void FloodWUPWakeupPacket::SetWakeUpSequence(Mac8Address wakeUpSequence) const {
        m_wakeUpSequence = wakeUpSequence;
    }




}