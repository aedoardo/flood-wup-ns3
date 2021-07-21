// routing

#ifndef FLOOD_WUP_ROUTING
#define FLOOD_WUP_ROUTING

#include "ns3/object.h"
#include "ns3/packet.h"
#include "flood-wup-wakeup-packet.h"

namespace ns3 {

    class FloodWUPRouting : public Object {

        public:
            static TypeId GetTypeId(void);
            FloodWUPRouting(void);
            virtual ~FloodWUPRouting(void);

            virtual void SetNetDevice(); // TODO: collegamento col NetDevice
            virtual void SetMac(); // TODO: MAC layer

            int SentPktCount() const;

        protected:
            virtual bool IsDst(const Ptr<Packet> p) const;
            virtual bool IsSrc(const Ptr<Packet> p) const;
            Ptr<FloodWUPWakeupPacket> CreateWakeUpPacket();
            virtual void SendPacket(Ptr<Packet> p);
            // TODO: getnetdevice e getmac

            virtual void DoDispose();

            int m_sentPktCount;


        
    };


}

#endif