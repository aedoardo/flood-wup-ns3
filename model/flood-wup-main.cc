
#include "flood-wup-main.h"
#include "ns3/node.h"

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("FloodWUPMain");

    NS_OBJECT_ENSURE_REGISTERED(FloodWUPMain);

    TypeId FloodWUPMain::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::FloodWUPMain")
            .SetParent<NetDevice>()
            .SetGroupName("FloodWUPMain"); // da continuare.

        return tid;
    }

    FloodWUPMain::FloodWUPMain() {
        NS_LOG_FUNCTION(this);
    }

    FloodWUPMain::~FloodWUPMain() {
        NS_LOG_FUNCTION(this);
    }

    void FloodWUPMain::DoDispose(void) {
        NS_LOG_FUNCTION(this);

        NetDevice::DoDispose();
    }

    void FloodWUPMain::SetState(MainState newState) {
        NS_LOG_FUNCTION(this);
        m_state = newState;
    }

    bool FloodWUPMain::IsOn(void) const {
        NS_LOG_FUNCTION(this);
        return m_state == MainState::ON;
    }

    bool FloodWUPMain::IsPointToPoint(void) const {
        NS_LOG_FUNCTION(this);
        return false;
    }

    bool FloodWUPMain::IsBridge(void) const {
        return false;
    }

    Address FloodWUPMain::GetMulticast(Ipv4Address multicastGroup) const {
        NS_ABORT_MSG("Unsupported");
        return Address();
    }
    
    bool FloodWUPMain::Send(Ptr<Packet> packet) {
        NS_LOG_FUNCTION(this << packet);
        // da implementare.
        return true;
    }

    bool FloodWUPMain::NeedsArp(void) const {
        NS_LOG_FUNCTION(this);
        return false;
    }

    bool FloodWUPMain::SupportsSendFrom(void) const {
        NS_LOG_FUNCTION(this);
        return false;
    }
}