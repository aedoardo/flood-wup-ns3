#ifndef FLOOD_WUP_DEVICE
#define FLOOD_WUP_DEVICE

#include <stdint.h>
#include <cstring>
#include "ns3/net-device.h"
#include "ns3/data-rate.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"


namespace ns3 {

    class FloodWUPDevice : public NetDevice {

        public:
            static TypeId GetTypeId();
            enum State {
                SLEEP,
                IDLE,
                TX,
                RX
            };
        
        FloodWUPDevice();

        virtual ~FloodWUPDevice();

        void NotifyTransmissionEnd(Ptr<const Packet>);

        void NotifyReceptionStart();

        void NotifyReceptionEndError();

        void NotifyReceptionEndOk (Ptr<Packet> p);

        void SetPhy (Ptr<Object> phy);

        Ptr<Object> GetPhy () const;

        void SetState(State s);

        State GetState() const;

        void WakeUp();

        virtual void SetIfIndex(const uint32_t index);
        virtual bool SetMtu(const uint16_t mtu);
        virtual void SetAddress(Address address);
        virtual uint16_t GetMtu(void) const;
        virtual Address GetAddress(void) const;
        virtual bool IsLinkUp (void) const;
        virtual void AddLinkChangeCallback(Callback<void> callback);
        virtual bool IsBroadcast(void) const;
        virtual Address GetBroadcast(void) const;
        virtual bool IsPointToPoint(void) const;
        virtual bool IsBridge(void) const;
        virtual bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
        virtual bool SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
        virtual Ptr<Node> GetNode(void) const;
        virtual void SetNode (Ptr<Node> node);
        virtual bool NeedsArp (void) const;
        virtual void SetReceiveCallbaack (NetDevice::ReceiveCallback cb);
        virtual Address GetMulticast (Ipv4Address addr) const;
        virtual Address GetMulticast (Ipv6Address addr) const;
        virtual void SetPromiscReceiveCCaaallback (PromiscReceiveCallback cb);
        virtual bool SupportsSendFrom(void) const;
        virtual bool IsMulticast(void) const;

    private:
        void StartTransmission();
        virtual void DoDispose(void);

        Mac48Address m_address; // MAC Address for the device.
        State m_state;
        Ptr<Node> m_node;
        Ptr<Object> m_phy;
        TracedCallback<> m_linkChangeCallbacks;
        NetDevice::ReceiveCallback m_rxCallback;
        
    };

}



#endif