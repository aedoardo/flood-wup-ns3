#include "main-radio-energy-model.h"

#include "wur-common-net-device.h"
#include "wur-common-phy.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("MainRadioEnergyModel");
void MainRadioEnergyModel::HandleEnergyDepletion(void) {
        NS_LOG_FUNCTION(this);
        NS_LOG_DEBUG("MainRadioEnergyModel:Energy is depleted at node #"
                     << m_node->GetId());
        // invoke energy depletion callback, if set.
        if (!m_energyDepletionCallback.IsNull()) {
                m_energyDepletionCallback();
        }
        // invoke the phy energy depletion handler
        Ptr<WurCommonNetDevice> dev =
            m_node->GetDevice(0)->GetObject<WurCommonNetDevice>();
        dev->GetMainRadioPhy()->EnergyDepletionHandler();
        SetMicroModemState(WurCommonPhy::WurCommonPhyState::DISABLED);
}

void MainRadioEnergyModel::HandleEnergyRecharged(void) {
        NS_LOG_FUNCTION(this);
        NS_LOG_DEBUG("RadioEnergyModel:Energy is recharged at node #"
                     << m_node->GetId());
        // invoke energy recharge callback, if set.
        if (!m_energyRechargeCallback.IsNull()) {
                m_energyRechargeCallback();
        }
        // invoke the phy energy recharge handler
        Ptr<WurCommonNetDevice> dev =
            m_node->GetDevice(0)->GetObject<WurCommonNetDevice>();
        dev->GetMainRadioPhy()->EnergyRechargeHandler();
        SetMicroModemState(WurCommonPhy::WurCommonPhyState::OFF);
}
}  // namespace ns3
