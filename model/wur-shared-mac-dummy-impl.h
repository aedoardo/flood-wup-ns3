#ifndef WUR_SHARED_MAC_DUMMY_IMPL_H
#define WUR_SHARED_MAC_DUMMY_IMPL_H

#include "ns3/header.h"
#include "ns3/mac8-address.h"
#include "ns3/wur-shared-mac.h"
namespace ns3 {
class WurSharedMacDummyImpl : public WurSharedMac {
       public:
        void SetAddress(Address);
        Address GetAddress() const;
        bool SupportsSendFrom(void) const { return true; }
        void StartWurTxMechanismImpl();
        void StartWurRxMechanismImpl();
        void OnDataRx(Ptr<Packet>);
        void OnWurRx(Ptr<Packet>);
        void OnWurTx(Ptr<Packet>);
        void StartDataTx();
        void StartDataRx();
        void SetPromisc(void);
        static TypeId GetTypeId(); 
        typedef enum {
           WakeUpPacket,
           DataPacket
        } WurPacketType; 

        class WurSharedMacDummyImplHeader : public Header {
                using Chunk::Deserialize;

               public:
                WurSharedMacDummyImplHeader(){};
                virtual ~WurSharedMacDummyImplHeader(){};
                static TypeId GetTypeId(void);
                virtual TypeId GetInstanceTypeId(void) const;
                virtual uint32_t GetSerializedSize(void) const;
                virtual void Serialize(Buffer::Iterator start) const;
                virtual uint32_t Deserialize(Buffer::Iterator start);
                virtual void Print(std::ostream &os) const;
                void SetFrom(uint8_t from) { m_from = Mac8Address(from); }
                void SetTo(uint8_t to) { m_to = Mac8Address(to); }
                void SetFrom(Address from) {
                        m_from = Mac8Address::ConvertFrom(from);
                }
                void SetTo(Address to) { m_to = Mac8Address::ConvertFrom(to); }
                void SetPacketType(WurPacketType type) { m_type = type; }
                void SetHeaderWakeUpSequence(std::string seq) { m_seq = seq; }
                WurPacketType GetPacketType() { return m_type; }
                std::string GetWakeUpSequenceHeader() { return m_seq; }
                Mac8Address GetFrom() { return m_from; }
                Mac8Address GetTo() { return m_to; }

               private:
                Mac8Address m_from;
                Mac8Address m_to;
                WurPacketType m_type;
                std::string m_seq;
        };

       private:
        Mac8Address m_address;
};
}  // namespace ns3

#endif /* WUR_SHARED_MAC_DUMMY_IMPL_H */
