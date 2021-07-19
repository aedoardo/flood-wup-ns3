#ifndef FLOOD_WUP_ROUTING
#define FLOOD_WUP_ROUTING

#include "ns3/ipv4-routing-protocol.h"

namespace ns3 {
    namespace floodwup {
        
        class RoutingProtocol : public Ipv4RoutingProtocol {

            public:
                static TypeId GetTypeId(void);
                static const uint32_t FloodWUP_PORT;

                RoutingProtocol();

                virtual ~RoutingProtocol();
                virtual void DoDispose();

                Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &socker);
                bool RouteInput(Ptr<const Packet> p, consst Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback ldc, ErrorCallback ecb);
                virtual void NotifyInterfaceUp(uint32_t interface);
                virtual void NotifyInterfaceDown(uint32_t interface);
                virtual void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address);
                virtual void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address);
                virtual void SetIpv4(Ptr<Ipv4> ipv4);
                virtual void PrintRoutingTable(Ptr<OutputStreamWrapper> stream) const;

            protected:
                virtual void DoInitialize(void);
            
            private:
            



        }
    }
}


#endif