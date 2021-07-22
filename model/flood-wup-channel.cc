#include "flood-wup-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/pointer.h"

namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("FloodWUPChannel");
    NS_OBJECT_ENSURE_REGISTERED(FloodWUPChannel);

    TypeId FloodWUPChannel::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::FloodWUPChannel")
            .SetParent<Channel>()
            .AddConstructor<FloodWUPChannel>()
            .AddAttribute("PropagationDelayModel", "A pointer to the propagation delay model attached to this channel.", 
            PointerValue(), 
            MakePointerAccessor(&FloodWUPChannel::m_delay),
            MakePointerChecker<PropagationDelayModel>());

        return tid;
    }

    FloodWUPChannel::FloodWUPChannel() {
        NS_LOG_FUNCTION(this);
    }

    FloodWUPChannel::~FloodWUPChannel() {
        NS_LOG_FUNCTION(this);
    }

    void FloodWUPChannel::SetPropagation(const Ptr<PropagationDelayModel> delay) {
        NS_LOG_FUNCTION(this);
        m_delay = delay;
    }

    void FloodWUPChannel::Send() {
        NS_LOG_FUNCTION(this);

        // TODO: da implementare.
    }

    void FloodWUPChannel::Receive() {
        NS_LOG_FUNCTION(this);
        //TODO: implementare.
    }

    std::size_t FloodWUPChannel::GetNDevices (void) const {
        return 0; // TODO: una lista da ritornare (la size).
    }

    Ptr<NetDevice> FloodWUPChannel::GetDevice(std::size_t i) const {
        // TODO: implementare, ritorna l'i-esimo device nella lista dei device.
        return NULL;
    }

    void FloodWUPChannel::Add() {
        NS_LOG_FUNCTION(this);
        // TODO: aggiunge un PHY
    }


}