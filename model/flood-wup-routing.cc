// routing


#include "flood-wup-routing.h"
#include "flood-wup-wakeup-packet.h"

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("FloodWUPRouting");
    NS_OBJECT_ENSURE_REGISTERED(FloodWUPRouting);

    FloodWUPRouting::FloodWUPRouting() {
        m_sentPktCount = 0;
    }

    TypeId FloodWUPRouting::GetTypeId() {
        static TypeId tid = TypeId("ns3:FloodWUPRouting")
            .SetParent<Object> ()
            .AddConstructor<FloodWUPRouting> ();
        
        return tid;
    }

    Ptr<FloodWUPWakeupPacket> FloodWUPRouting::CreateWakeUpPacket() {
        Ptr<FloodWUPWakeupPacket> pkt = Create<FloodWUPWakeupPacket>();

        if(pkt == NULL)
            return NULL;

        //pkt->SetWakeUpSequence() // TODO: setto la wakeup sequence
        // pkt->SetSequenceNumber() // TODOO: setto il sequence number. 
        
        m_sentPktCount += 1; // incremento i pacchetti inviati.
        return pkt;
    }
}