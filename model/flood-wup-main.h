#ifndef FLOOD_WUP_MAIN
#define FLOOOD_WUP_MAIN


#include "ns3/net-device.h"

namespace ns3
{

    class FloodWUPMain : public NetDevice
    {

        public:

            static TypeId GetTypeId (void);
            enum MainState {
                ON,
                OFF
            };

            FloodWUPMain();
            virtual ~FloodWUPMain();

            bool IsOn(void) const;
            void SetState(MainState newState);

            // da NetDevice
            virtual void SetIfIndex (const uint32_t index);
            virtual uint32_t GetIfIndex (void) const;
            virtual Ptr<Channel> GetChannel (void) const;
            virtual void SetAddress (Address address);
            virtual Address GetAddress(void) const;

            virtual bool SetMtu(const uint16_t mtu);
            virtual bool IsLinkUp(void) const;
            virtual void AddLinkChangeCallback (Callback<void> cb);
            virtual bool IsBroadcast(void) const;
            virtual Address GetBroadcast (void) const;
            virtual bool IsMulticast (void) const;
            virtual Address GetMulticast(Ipv4Address multicastGroup) const; // non si usa IPv4.
            virtual bool IsPointToPoint(void) const;
            virtual bool IsBridge(void) const;
            virtual bool Send(Ptr<Packet> packet); // da implementare per la main antenna.
            virtual bool SendFrom(); // da implementare
            virtual bool SupportsSendFrom (void) const;
            virtual Ptr<Node> GetNode (void) const;
            virtual void SetNode(Ptr<Node> node);
            virtual bool NeedsArp(void) const;
            virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
            virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);


        private:
            virtual void DoDispose(void);
            virtual void DoInitialize(void);

            bool m_state = MainState::OFF;
            


    ;
    };
    
    
    
} // namespace ns3


#endif