#include "flood-wup-device.h"

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("FloodWUPDevice");


    std::ostream& operator<< (std::ostream& os, FloodWUPDevice::State state) {
        switch(state) {
            case FloodWUPDevice::SLEEP:
                os << "SLEEP";
                break;
            
            case FloodWUPDevice::IDLE:
                os << "IDLE";
                break;

            case FloodWUPDevice::TX:
                os << "TX";
                break;

            case FloodWUPDevice::RX:
                os << "RX";
                break;
        }
    };

    NS_OBJECT_ENSURE_REGISTERED(FloodWUPDevice);

    TypeId FloodWUPDevice::GetTypeId(void) {

        // da completare quanto implementato PHY e MAC.

        static TypeId tid = TypeId("ns3::FloodWUPNetDevice")
            .SetParent<NetDevice>()
            .SetGroupName("FloodWUP")
            .AddConstructor<FloodWUPDevice>();

        return tid;
    }

    FloodWUPDevice::FloodWUPDevice() : m_state (SLEEP) {
        NS_LOG_FUNCTION(this);
    }

    FloodWUPDevice::~FloodWUPDevice() {
        NS_LOG_FUNCTION(this);
    }

    void FloodWUPDevice::DoDispose() {
        NS_LOG_FUNCTION(this);
        
        // da implementare

        NetDevice::DoDispose();
    }

    void FloodWUPDevice::SetAddress(Address address) {
        NS_LOG_FUNCTION(this);
        m_address = Mac48Address::ConvertFrom(address);
    }

    Address FloodWUPDevice::GetAddress(void) const {
        NS_LOG_FUNCTION(this);
        return m_address;
    }

    bool FloodWUPDevice::IsBroadcast (void) const {
        NS_LOG_FUNCTION(this);

        return true;
    }

    Address FloodWUPDevice::GetBroadcast (void) const {
        NS_LOG_FUNCTION(this);

        return Mac48Address("ff:ff:ff:ff:ff:ff");
    }

    bool FloodWUPDevice::IsMulticast (void) const {
        NS_LOG_FUNCTION(this);;

        return true;
    }

    Address FloodWUPDevice::GetMulticast (Ipv4Address addr) const {
        // in WUP non serve?

        NS_LOG_FUNCTION(addr);
        Mac48Address ad = Mac48Address::GetMulticast(addr);
        return ad;
    }


    bool FloodWUPDevice::IsPointToPoint (void) const {
        NS_LOG_FUNCTION(this);
        return false;
    }

    bool FloodWUPDevice::IsBridge (void) const {
        NS_LOG_FUNCTION(this);
        return false;
    }

    Ptr<Node> FloodWUPDevice::GetNode (void) const {
        NS_LOG_FUNCTION(this);
        return m_node;
    }

    void FloodWUPDevice::SetNode(Ptr<Node> node) {
        NS_LOG_FUNCTION(node);
        m_node = node;
    }


    void FloodWUPDevice::SetPhy(Ptr<Object> phy) {
        // da rivedere quando implemento il PHY
        NS_LOG_FUNCTION(this << phy);
        m_phy = phy;
    }

    Ptr<Object> FloodWUPDevice::GetPhy() const {
        NS_LOG_FUNCTION(this);
        return m_phy;
    }

    bool FloodWUPDevice::NeedsArp(void) const {
        NS_LOG_FUNCTION(this);
        return false; // teoricamente non c'è bisogno di ARP.
    }

    bool FloodWUPDevice::IsLinkUp(void) const {
        NS_LOG_FUNCTION(this);
        return true; // Il link è UP solo nel caso in cui siamo IDLE? Anche TX e RX?
    }

    void FloodWUPDevice::AddLinkChangeCallback(Callback<void> cb) {
        NS_LOG_FUNCTION(&cb);
        m_linkChangeCallbacks.ConnectWithoutContext(cb);
    }

    void FloodWUPDevice::SetReceiveCallbaack(NetDevice::ReceiveCallback cb) {
        NS_LOG_FUNCTION(&cb);
        m_rxCallback = cb;
    }

    bool FloodWUPDevice::SupportsSendFrom() const {
        NS_LOG_FUNCTION(this);
        return true;
    }

    bool FloodWUPDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) {
        // protocol number in questo caso inutile
        NS_LOG_FUNCTION(packet << dest);
        return SendFrom(packet, m_address, dest, protocolNumber);
    }

    bool FloodWUPDevice::SendFrom(Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber) {
        // va implementato l'invio del pacchetto agli altri nodi.
        return true;
    }


}