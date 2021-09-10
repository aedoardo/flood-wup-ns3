#include "wur-shared-mac.h"

#include <utility>

#include "ns3/log-macros-enabled.h"
#include "ns3/log.h"
#include "wur-common-net-device.h"
#include "wur-common-phy.h"
#include "flood-wakeup-packet.h"
#include "wur-shared-mac-dummy-impl.h"
#include "wur-common-phy.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurSharedMac");
void WurSharedMac::SetNetDevice(Ptr<WurCommonNetDevice> netDevice) {
        m_netDevice = netDevice;
}
Ptr<WurCommonNetDevice> WurSharedMac::GetNetDevice() const {
        return m_netDevice;
}
Ptr<WurCommonPhy> WurSharedMac::GetMainRadioPhy() const {
        return m_netDevice->GetMainRadioPhy();
}
Ptr<WurCommonPhy> WurSharedMac::GetWurRadioPhy() const {
        return m_netDevice->GetWurRadioPhy();
}

void WurSharedMac::Enqueue(Ptr<Packet> packet, Address to) {
        m_txqueue.push_back(std::make_pair(packet, to));
        NS_LOG_FUNCTION(packet);
        NS_LOG_INFO("Packets in queue: " + std::to_string(m_txqueue.size()));

        if (m_state == WurSharedMacState::IDLE) {
                StartWurTxMechanism();
        }
}
 
void WurSharedMac::StartWurTxMechanism() {
        NS_LOG_FUNCTION("Starting WUR transmitting mechanism");
        if (GetWurRadioPhy()->GetState() !=
            WurCommonPhy::WurCommonPhyState::DISABLED) {
                wurSendingTimer.Schedule(WUR_RX_MECHANISM_TIMEOUT);
                
                m_state = WurSharedMacState::WUR_TX_MECHANISM;
                StartWurTxMechanismImpl();
        } else 
                NS_LOG_FUNCTION("Can't start wur mechanism since wur is disabled (out of energy)");
}

void WurSharedMac::StartWurRxMechanism() {
        NS_LOG_FUNCTION("Starting WUR receiving mechanism");
        wurReceivingTimer.Schedule(WUR_TX_MECHANISM_TIMEOUT);
        m_state = WurSharedMacState::WUR_RX_MECHANISM;
        // ex. answer to a wur packet
        StartWurRxMechanismImpl();
}

void WurSharedMac::TimerWurTxMechanismCallback() {
        NS_LOG_FUNCTION_NOARGS();
        // failure of previous wur mechanism or idle, try to send data
        if ((m_state == WurSharedMacState::WUR_TX_MECHANISM ||
             m_state == WurSharedMacState::IDLE) &&
            !m_txqueue.empty()) {
                StartWurTxMechanism();
                NS_LOG_FUNCTION(
                    "restarting to "
                    "send a new packet");
        } else {
                NS_LOG_FUNCTION(
                    "no new packet or "
                    "doing smthng else ");
        }
}

void WurSharedMac::TimerWurRxMechanismCallback() {
        NS_LOG_FUNCTION_NOARGS();
        // failure of previous wur rx mechanism or idle, try to send data
        if (m_state == WurSharedMacState::WUR_RX_MECHANISM ||
            m_state == WurSharedMacState::IDLE) {
                GetMainRadioPhy()->TurnOff();
                m_state = WurSharedMacState::IDLE;
        }
}
void WurSharedMac::TimerDataReceivingCallback() {
        // turn off
        if (m_state == WurSharedMacState::RECEIVING_DATA) {
                // turning off phy, possibly truncating rx packet
                GetMainRadioPhy()->TurnOff();
                m_state = WurSharedMacState::IDLE;
        }
        // TODO: check if I need to tx something (?)
}

void WurSharedMac::StartDataRxWrapper() {
        NS_LOG_FUNCTION_NOARGS();
        m_state = WurSharedMacState::RECEIVING_DATA;
        dataReceivingTimer.Schedule(DATA_RECEPTION_TIMEOUT);
        StartDataRx();
}

void WurSharedMac::OnWurTxMechanismSuccess() {
        // If no packet reception activated in the meanwhile and wurSendingTimer
        // is running
        NS_LOG_FUNCTION_NOARGS();
        if (m_state == WurSharedMacState::WUR_TX_MECHANISM &&
            wurSendingTimer.IsRunning()) {
                NS_LOG_FUNCTION("WUR mechanism started successfully");
                wurSendingTimer.Remove();
                StartDataTxWrapper();
        }
}

void WurSharedMac::OnWurRxMechanismSuccess() {
        if (m_state == WurSharedMacState::WUR_RX_MECHANISM &&
            wurReceivingTimer.IsRunning()) {
                wurReceivingTimer.Remove();
                StartDataRxWrapper();
        }
}
void WurSharedMac::StartDataTxWrapper() {
        NS_LOG_FUNCTION_NOARGS();
        if (GetMainRadioPhy()->GetState() !=
            WurCommonPhy::WurCommonPhyState::DISABLED) {
                dataSendingTimer.Schedule(DATA_TRANSMISSION_TIMEOUT);
                m_state = WurSharedMacState::SENDING_DATA;
                StartDataTx();
        } else
                NS_LOG_FUNCTION(
                    "Can't send packet because radio is disabled (out of "
                    "energy)");
}

void WurSharedMac::TimerDataSendingCallback() {
        NS_LOG_FUNCTION_NOARGS();
        // turn off phy and go to idle
        m_state = WurSharedMacState::IDLE;
        GetMainRadioPhy()->TurnOff();
        // start wurSendingTimer if something else is in tx queue
        if (m_txqueue.size() > 0) StartWurTxMechanism();
}

void WurSharedMac::NotifyTx(Ptr<Packet> packet) {
        NS_LOG_FUNCTION(packet);
        m_macTxTrace(packet);
}

void WurSharedMac::ReceiveWurPacket(Ptr<WurCommonPsdu> psdu) {
        FloodWUPPacketHeader header;
        psdu->GetPayload()->PeekHeader(header);
        Mac16Address wakeUpSequence = header.GetWakeUpSequence();

        NS_LOG_DEBUG("Received wake-up packet with wake up sequence: " << wakeUpSequence);
        if(wakeUpSequence == m_netDevice->GetWakeUpSequence()) {
                // qui riceviamo la wake up sequence per questo device
                m_netDevice->GetMainRadioPhy()->TurnOn(); // accendiamo la main radio
                NS_LOG_DEBUG("Turned on main radio.");
        }
}

void WurSharedMac::ReceivedData(Ptr<WurCommonPsdu> psdu) {
        
        WurSharedMacDummyImpl::WurSharedMacDummyImplHeader header;
        psdu->GetPayload()->PeekHeader(header);
        if(Mac8Address::ConvertFrom(m_netDevice->GetAddress()) == header.GetTo()) {
                NS_LOG_DEBUG("Received data packet for device with wus: " << m_netDevice->GetWakeUpSequence());
                // qui spegniamo la main radio e la wake up radio in IDLE del device.
                if(m_netDevice->GetLastPacketReceived().count(header.GetFrom()) == 0) {
                        NS_LOG_DEBUG("First time that we receive a packet from the sender.");
                        m_netDevice->lastPacketReceived.insert({header.GetFrom(), psdu->GetPacketId()});
                        m_netDevice->GetMainRadioPhy()->SetRxOkCallback(MakeCallback(&WurSharedMac::OnDataRx, m_netDevice->GetSharedMac()));
                } else {
                        NS_LOG_DEBUG("Checking if is a duplicate.");
                        uint16_t lastId = m_netDevice->GetLastPacketReceived().find(header.GetFrom())->second;
                        if(lastId == psdu->GetPacketId()) {
                                NS_LOG_DEBUG("Received packet is a duplicate!");
                        } else {
                                m_netDevice->lastPacketReceived.insert({header.GetFrom(), psdu->GetPacketId()});
                                NS_LOG_DEBUG("Received NEW packet from the sender, updated its value.");
                                m_netDevice->GetMainRadioPhy()->SetRxOkCallback(MakeCallback(&WurSharedMac::OnDataRx, m_netDevice->GetSharedMac()));
                        }
                }
                m_netDevice->GetMainRadioPhy()->TurnOff();
                m_netDevice->GetWurRadioPhy()->ChangeState(WurCommonPhy::WurCommonPhyState::IDLE);
                // advance wake up sequence!
                m_netDevice->AdvanceWakeUpSequence();
        }


}

void WurSharedMac::Initialize() {
        wurSendingTimer.SetFunction(&WurSharedMac::TimerWurTxMechanismCallback,
                                    this);
        wurReceivingTimer.SetFunction(
            &WurSharedMac::TimerWurRxMechanismCallback, this);
        dataSendingTimer.SetFunction(&WurSharedMac::TimerDataSendingCallback,
                                     this);
        dataReceivingTimer.SetFunction(
            &WurSharedMac::TimerDataReceivingCallback, this);
}

TypeId WurSharedMac::GetTypeId() {
        static TypeId tid =
            TypeId("ns3::WurSharedMac").SetParent<Object>().SetGroupName("Wur");
        return tid;
}

}  // namespace ns3
