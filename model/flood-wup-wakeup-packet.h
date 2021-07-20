#ifndef FLOOD_WUP_WAKEUP_PACKET
#define FLOOD_WUP_WAKEUP_PACKET

#include "ns3/header.h"
#include "ns3/mac8-address.h"

namespace ns3 {

    class FloodWUPWakeupPacket : public Header {

        public:
            static TypeId GetTypeId(void);
            FloodWUPWakeupPacket();
            virtual ~FloodWUPWakeupPacket();
            virtual uint32_t GetSerializedSize(void) const;
            virtual void Serialize (Buffer::Iterator start) const;
            virtual uint32_t Deserialize(Buffer::Iterator start);
            virtual void Print(std::ostream &os) const;

            uint16_t GetLength() const;
            uint16_t GetSequenceNumber() const;
            void SetSequenceNumber(uint16_t sequenceNumber);
            Mac8Address GetWakeUpSequence() const;
            void SetWakeUpSequence(Mac8Address wakeUpSequence); // da vedere se lasciare ad esempio Mac8Address.

        private:
            uint16_t m_sequenceNumber;
            uint16_t m_length;
            Mac8Address m_wakeUpSequence;

    };

}

#endif