#ifndef FLOOD_WAKEUP_PACKET
#define FLOOD_WAKEUP_PACKET

#include "ns3/header.h"
#include "ns3/mac16-address.h"

namespace ns3 {
    class FloodWUPPacketHeader : public Header {
 
        public:
            FloodWUPPacketHeader() {};
            virtual ~FloodWUPPacketHeader() {};
            static TypeId GetTypeId(void);
            virtual TypeId GetInstanceTypeId(void) const;
            virtual uint32_t GetSerializedSize(void) const;
            virtual void Serialize(Buffer::Iterator start) const;
            virtual uint32_t Deserialize(Buffer::Iterator start);
            virtual void Print(std::ostream &os) const;
            void SetHeaderWakeUpSequence(Mac16Address seq) {
                wakeUpSequence = seq;
            };
            Mac16Address GetWakeUpSequence() {
                return wakeUpSequence;
            }

        private:
            Mac16Address wakeUpSequence;
    };
}

#endif