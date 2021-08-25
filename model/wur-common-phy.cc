#include "wur-common-phy.h"

#include "ns3/device-energy-model.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "wur-common-channel.h"
#include "wur-common-net-device.h"
#include "wur-common-ppdu.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurCommonPhy");
Ptr<WurCommonChannel> WurCommonPhy::GetChannel() const { return m_channel; }
Ptr<MobilityModel> WurCommonPhy::GetMobility() const { return m_mobility; }
Ptr<WurCommonNetDevice> WurCommonPhy::GetDevice() const { return m_netDevice; }

void WurCommonPhy::StartReceivePreamble(Ptr<WurCommonPpdu> ppdu,
                                        double rxPowerDbm) {
        NS_LOG_FUNCTION(this << rxPowerDbm);
        if (ppdu->IsTruncatedTx()) {
                NS_LOG_DEBUG(
                    "Packet reception stopped because transmitter has been "
                    "switched off");
                return;
        }
        switch (GetState()) {
                case WurCommonPhyState::RX:
                        NS_LOG_DEBUG("Drop packet because already in Rx (power="
                                     << rxPowerDbm << "dBm)");
                        NotifyRxDrop(ppdu, "Already receiving packet");
                        break;
                case WurCommonPhyState::TX:
                        NS_LOG_DEBUG("Drop packet because already in Tx (power="
                                     << rxPowerDbm << "dBm)");
                        NotifyRxDrop(ppdu, "Already in Tx");
                        break;
                case WurCommonPhyState::IDLE:
                        NS_LOG_INFO("Start receiving");
                        ChangeState(WurCommonPhyState::RX);
                        SetRxPacket(ppdu);
                        Simulator::Schedule(m_preambleDuration,
                                            &WurCommonPhy::StartRx, this, ppdu,
                                            rxPowerDbm);
                        break;
                case WurCommonPhyState::OFF:
                        NS_LOG_DEBUG("Drop packet because in sleep mode");
                        NotifyRxDrop(ppdu, "Sleeping");
                        break;
                case WurCommonPhyState::DISABLED:
                        NS_LOG_DEBUG("Drop packet as radio is out of energy");
                        NotifyRxDrop(ppdu, "Disabled");
                        break;
                default:
                        NS_FATAL_ERROR("Invalid WurCommonPhy state.");
                        break;
        }
}

void WurCommonPhy::TurnOn() {
        NS_LOG_FUNCTION_NOARGS();
        if (m_state == WurCommonPhyState::OFF) {
                ChangeState(WurCommonPhyState::IDLE);
        }
}

void WurCommonPhy::TurnOff() {
        NS_LOG_FUNCTION_NOARGS();
        if (m_state != WurCommonPhyState::OFF && m_state != WurCommonPhyState::DISABLED) {
                ChangeState(WurCommonPhyState::OFF);
                if (m_rxPacket != nullptr) m_rxPacket->SetTruncatedRx();
                if (m_txPacket != nullptr) m_txPacket->SetTruncatedTx();
        }
}
void WurCommonPhy::SetMobility(Ptr<MobilityModel> mobility) {
        m_mobility = mobility;
}

TypeId WurCommonPhy::GetTypeId() {
        static TypeId tid =
            TypeId("ns3::WurCommonPhy").SetParent<Object>().SetGroupName("Wur");
        return tid;
}
void WurCommonPhy::SetChannel(Ptr<WurCommonChannel> channel) {
        m_channel = channel;
}
void WurCommonPhy::SetDevice(Ptr<WurCommonNetDevice> device) {
        m_netDevice = device;
}

void WurCommonPhy::SetEnergyModelCallback(
    DeviceEnergyModel::ChangeStateCallback cb) {
        m_energyModelCallback = cb;
}
 
void WurCommonPhy::ChangeState(WurCommonPhy::WurCommonPhyState state) {
        NS_LOG_FUNCTION(state);
        //can't set disabled state with ChangeState, must be manually done in the depletion handler
        NS_ASSERT(state != DISABLED);
        if (!m_energyModelCallback.IsNull()) m_energyModelCallback(state);
        //I could be disabled due to previous function call
        //Hence, I only set new state if I wasn't disabled
        if(m_state != DISABLED)
                m_state = state;
        NS_LOG_FUNCTION("Final state" << m_state);
}

/**
 * Invoked when we're out of energy
 */
void WurCommonPhy::EnergyDepletionHandler() {
        NS_LOG_FUNCTION_NOARGS();
        m_state = WurCommonPhyState::DISABLED;
        if(m_rxPacket != nullptr)
                m_rxPacket->SetTruncatedRx();
        if(m_txPacket != nullptr)
                m_txPacket->SetTruncatedTx();
}
/**
 * Invoked when getting out of disabled mode
 */
void WurCommonPhy::EnergyRechargeHandler() {
        //going to off state
        ChangeState(WurCommonPhyState::OFF);
}
        
}  // namespace ns3
