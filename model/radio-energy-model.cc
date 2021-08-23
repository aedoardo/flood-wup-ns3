#include "radio-energy-model.h"

#include "ns3/double.h"
#include "ns3/energy-source.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uan-net-device.h"
#include "ns3/uan-phy.h"
#include "wur-common-phy.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("RadioEnergyModel");

NS_OBJECT_ENSURE_REGISTERED(RadioEnergyModel);

TypeId RadioEnergyModel::GetTypeId(void) {
        static TypeId tid =
            TypeId("ns3::RadioEnergyModel")
                .SetParent<DeviceEnergyModel>()
                //.AddConstructor<RadioEnergyModel>()
                .AddAttribute(
                    "TxPowerW", "The radio Tx power in Watts", DoubleValue(50),
                    MakeDoubleAccessor(&RadioEnergyModel::SetTxPowerW,
                                       &RadioEnergyModel::GetTxPowerW),
                    MakeDoubleChecker<double>())
                .AddAttribute(
                    "RxPowerW", "The radio Rx power in Watts",
                    DoubleValue(0.158),
                    MakeDoubleAccessor(&RadioEnergyModel::SetRxPowerW,
                                       &RadioEnergyModel::GetRxPowerW),
                    MakeDoubleChecker<double>())
                .AddAttribute(
                    "IdlePowerW", "The radio Idle power in Watts",
                    DoubleValue(0.158),
                    MakeDoubleAccessor(&RadioEnergyModel::SetIdlePowerW,
                                       &RadioEnergyModel::GetIdlePowerW),
                    MakeDoubleChecker<double>())
                .AddAttribute(
                    "SleepPowerW", "The radio Sleep power in Watts",
                    DoubleValue(0.0058),
                    MakeDoubleAccessor(&RadioEnergyModel::SetSleepPowerW,
                                       &RadioEnergyModel::GetSleepPowerW),
                    MakeDoubleChecker<double>())
                .AddTraceSource(
                    "TotalEnergyConsumption",
                    "Total energy consumption of the radio device.",
                    MakeTraceSourceAccessor(
                        &RadioEnergyModel::m_totalEnergyConsumption),
                    "ns3::TracedValueCallback::Double");
        return tid;
}

RadioEnergyModel::RadioEnergyModel() {
        NS_LOG_FUNCTION(this);
        m_currentState = WurCommonPhy::WurCommonPhyState::OFF;  // initially OFF
        m_lastUpdateTime = Seconds(0.0);
        m_energyDepletionCallback.Nullify();
        m_node = 0;
        m_source = 0;
}

RadioEnergyModel::~RadioEnergyModel() {}

void RadioEnergyModel::SetNode(Ptr<Node> node) {
        NS_LOG_FUNCTION(this << node);
        NS_ASSERT(node != 0);
        m_node = node;
}

Ptr<Node> RadioEnergyModel::GetNode(void) const { return m_node; }

void RadioEnergyModel::SetEnergySource(Ptr<EnergySource> source) {
        NS_LOG_FUNCTION(this << source);
        NS_ASSERT(source != 0);
        m_source = source;
}

double RadioEnergyModel::GetTotalEnergyConsumption(void) const {
        NS_LOG_FUNCTION(this);
        return m_totalEnergyConsumption;
}

double RadioEnergyModel::GetTxPowerW(void) const {
        NS_LOG_FUNCTION(this);
        return m_txPowerW;
}

void RadioEnergyModel::SetTxPowerW(double txPowerW) {
        NS_LOG_FUNCTION(this << txPowerW);
        m_txPowerW = txPowerW;
}

double RadioEnergyModel::GetRxPowerW(void) const {
        NS_LOG_FUNCTION(this);
        return m_rxPowerW;
}

void RadioEnergyModel::SetRxPowerW(double rxPowerW) {
        NS_LOG_FUNCTION(this << rxPowerW);
        m_rxPowerW = rxPowerW;
}

double RadioEnergyModel::GetIdlePowerW(void) const {
        NS_LOG_FUNCTION(this);
        return m_idlePowerW;
}

void RadioEnergyModel::SetIdlePowerW(double idlePowerW) {
        NS_LOG_FUNCTION(this << idlePowerW);
        m_idlePowerW = idlePowerW;
}

double RadioEnergyModel::GetSleepPowerW(void) const {
        NS_LOG_FUNCTION(this);
        return m_sleepPowerW;
}

void RadioEnergyModel::SetSleepPowerW(double sleepPowerW) {
        NS_LOG_FUNCTION(this << sleepPowerW);
        m_sleepPowerW = sleepPowerW;
}

int RadioEnergyModel::GetCurrentState(void) const {
        NS_LOG_FUNCTION(this);
        return m_currentState;
}

void RadioEnergyModel::SetEnergyDepletionCallback(
    RadioEnergyDepletionCallback callback) {
        NS_LOG_FUNCTION(this);
        if (callback.IsNull()) {
                NS_LOG_DEBUG(
                    "RadioEnergyModel:Setting NULL energy depletion callback!");
        }
        m_energyDepletionCallback = callback;
}

void RadioEnergyModel::SetEnergyRechargeCallback(
    RadioEnergyRechargeCallback callback) {
        NS_LOG_FUNCTION(this);
        if (callback.IsNull()) {
                NS_LOG_DEBUG(
                    "RadioEnergyModel:Setting NULL energy recharge callback!");
        }
        m_energyRechargeCallback = callback;
}

void RadioEnergyModel::ChangeState(int newState) {
        NS_LOG_FUNCTION(this << newState);
        // NS_ASSERT (IsStateTransitionValid ((MicroModemState) newState));

        Time duration = Simulator::Now() - m_lastUpdateTime;
        NS_ASSERT(duration.GetNanoSeconds() >=
                  0);  // check if duration is valid

        // energy to decrease = current * voltage * time
        double energyToDecrease = 0.0;

        switch (m_currentState) {
                case WurCommonPhy::WurCommonPhyState::TX:
                        energyToDecrease = duration.GetSeconds() * m_txPowerW;
                        break;
                case WurCommonPhy::WurCommonPhyState::RX:
                        energyToDecrease = duration.GetSeconds() * m_rxPowerW;
                        break;
                case WurCommonPhy::WurCommonPhyState::IDLE:
                        energyToDecrease = duration.GetSeconds() * m_idlePowerW;
                        break;
                case WurCommonPhy::WurCommonPhyState::OFF:
                        energyToDecrease =
                            duration.GetSeconds() * m_sleepPowerW;
                        break;
                case WurCommonPhy::WurCommonPhyState::DISABLED:
                        energyToDecrease = 0;
                        break;
                default:
                        NS_FATAL_ERROR(
                            "RadioEnergyModel:Undefined radio state!");
        }

        // update total energy consumption
        m_totalEnergyConsumption += energyToDecrease;
        NS_LOG_FUNCTION("Energy consumed" <<  energyToDecrease << "in time" << duration.GetSeconds() << "previous state" << m_currentState);

        // update last update time stamp
        m_lastUpdateTime = Simulator::Now();

        // notify energy source
        m_source->UpdateEnergySource();

        //change state only if we're not out of energy
        if(m_currentState != WurCommonPhy::WurCommonPhyState::DISABLED)
                SetMicroModemState(newState);

        // some debug message
        NS_LOG_DEBUG("RadioEnergyModel:Total energy consumption at node #"
                     << m_node->GetId() << " is " << m_totalEnergyConsumption
                     << "J");
}

void RadioEnergyModel::HandleEnergyChanged(void) {
        NS_LOG_FUNCTION(this);
        // Not implemented
}

/*
 * Private functions start here.
 */

void RadioEnergyModel::DoDispose(void) {
        NS_LOG_FUNCTION(this);
        m_node = 0;
        m_source = 0;
        m_energyDepletionCallback.Nullify();
}

double RadioEnergyModel::DoGetCurrentA(void) const {
        NS_LOG_FUNCTION(this);
        double supplyVoltage = m_source->GetSupplyVoltage();
        NS_ASSERT(supplyVoltage != 0.0);
        double stateCurrent = 0.0;
        switch (m_currentState) {
                case WurCommonPhy::WurCommonPhyState::TX:
                        stateCurrent = m_txPowerW / supplyVoltage;
                        break;
                case WurCommonPhy::WurCommonPhyState::RX:
                        stateCurrent = m_rxPowerW / supplyVoltage;
                        break;
                case WurCommonPhy::WurCommonPhyState::IDLE:
                        stateCurrent = m_idlePowerW / supplyVoltage;
                        break;
                case WurCommonPhy::WurCommonPhyState::OFF:
                        stateCurrent = m_sleepPowerW / supplyVoltage;
                        break;
                case WurCommonPhy::WurCommonPhyState::DISABLED:
                        stateCurrent = 0.0;
                        break;
                default:
                        NS_FATAL_ERROR(
                            "RadioEnergyModel:Undefined radio state!");
        }

        return stateCurrent;
}

bool RadioEnergyModel::IsStateTransitionValid(const int destState) {
        NS_LOG_FUNCTION(this << destState);
        return true;
}

void RadioEnergyModel::SetMicroModemState(const int state) {
        NS_LOG_FUNCTION(this);
        if (IsStateTransitionValid(state)) {
                m_currentState = state;
                std::string stateName;
                switch (state) {
                        case WurCommonPhy::WurCommonPhyState::TX:
                                stateName = "TX";
                                break;
                        case WurCommonPhy::WurCommonPhyState::RX:
                                stateName = "RX";
                                break;
                        case WurCommonPhy::WurCommonPhyState::IDLE:
                                stateName = "IDLE";
                                break;
                        case WurCommonPhy::WurCommonPhyState::OFF:
                                stateName = "OFF";
                                break;
                        case WurCommonPhy::WurCommonPhyState::DISABLED:
                                stateName = "DISABLED";
                                break;
                }
                NS_LOG_DEBUG("RadioEnergyModel:Switching to state: "
                             << stateName << " at time = " << Simulator::Now());
        } else {
                NS_FATAL_ERROR("RadioEnergyModel:Invalid state transition!");
        }
}

}  // namespace ns3
