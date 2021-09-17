#include "wur-common-channel.h"
#include <bits/c++config.h>
#include <algorithm>
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/ptr.h"
#include "ns3/simulator.h"
#include "wur-common-net-device.h"
#include "wur-common-phy.h"
#include "wur-shared-mac-dummy-impl.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurCommonChannel");
void WurCommonChannel::SetPropagationDelayModel(
    const Ptr<PropagationDelayModel> delay) {
	m_delay = delay;
}
void WurCommonChannel::SetPropagationLossModel(
    const Ptr<PropagationLossModel> loss) {
	m_loss = loss;
}
std::size_t WurCommonChannel::GetNDevices() const { return m_phyList.size(); }
Ptr<NetDevice> WurCommonChannel::GetDevice(std::size_t i) const {
	return m_phyList[i]->GetDevice()->GetObject<NetDevice>();
}
TypeId WurCommonChannel::GetTypeId(void) {
	static TypeId tid =
	    TypeId("ns3::WurCommonChannel")
		.SetParent<Channel>()
		.SetGroupName("WakeUpRadio")
		.AddConstructor<WurCommonChannel>()
		.AddAttribute("PropagationLossModel",
			      "A pointer to the propagation loss model "
			      "attached to this channel.",
			      PointerValue(),
			      MakePointerAccessor(&WurCommonChannel::m_loss),
			      MakePointerChecker<PropagationLossModel>())
		.AddAttribute("PropagationDelayModel",
			      "A pointer to the propagation delay model "
			      "attached to this channel.",
			      PointerValue(),
			      MakePointerAccessor(&WurCommonChannel::m_delay),
			      MakePointerChecker<PropagationDelayModel>());
	return tid;
}
void WurCommonChannel::Send(Ptr<WurCommonPhy> sender,
			    Ptr<const WurCommonPpdu> ppdu,
			    double txPowerDbm) const {
	NS_LOG_FUNCTION(this << sender << ppdu << txPowerDbm);
	Ptr<MobilityModel> senderMobility = sender->GetMobility();
	NS_ASSERT(senderMobility != 0);

	for (PhyList::const_iterator i = m_phyList.begin();
	     i != m_phyList.end(); i++) {
		if (sender != (*i)) {
			NS_LOG_INFO("Found a PHY");
			//Ptr<MobilityModel> receiverMobility =
			//    (*i)->GetMobility()->GetObject<MobilityModel>();
			Ptr<MobilityModel> receiverMobility = (*i)->GetMobility();

			NS_LOG_INFO("Receiver Mobility OK!");

			Time delay =
			    m_delay->GetDelay(senderMobility, receiverMobility);

			double rxPowerDbm = m_loss->CalcRxPower(
			    txPowerDbm, senderMobility, receiverMobility);
			/*NS_LOG_DEBUG(
			    "propagation: txPower="
			    << txPowerDbm << "dbm, rxPower=" << rxPowerDbm
			    << "dbm, "
			    << "distance="
			    << senderMobility->GetDistanceFrom(receiverMobility)
			    << "m, delay=" << delay);*/

			
			Ptr<WurCommonPpdu> copy = Copy(ppdu);
			Ptr<NetDevice> dstNetDevice = (*i)->GetDevice();


			//NS_LOG_DEBUG("Sending a packet of type: " << copy->GetPsdu()->GetPacketType());

			uint32_t dstNode;
			if (dstNetDevice == 0) {
				dstNode = 0xffffffff;
			} else {
				dstNode = dstNetDevice->GetNode()->GetId();
			}


			Simulator::ScheduleWithContext(
			    dstNode, delay, &WurCommonChannel::Receive, 
			    (*i), copy, rxPowerDbm);
		}
	}
}
/**
 * \brief Basic reception, check if the phy sensitivity thresold is high enough
 * to receive the signal
 * @param receiver
 * @param ppdu
 * @param rxPowerDbm
 */
void WurCommonChannel::Receive(Ptr<WurCommonPhy> receiver,
			       Ptr<WurCommonPpdu> ppdu, double rxPowerDbm) {
	//NS_LOG_FUNCTION(receiver << ppdu << rxPowerDbm);
	if ((rxPowerDbm + receiver->GetRxGain()) <
	    receiver->GetRxSensitivity()) {
		NS_LOG_INFO("Received signal too weak to process: "
			    << rxPowerDbm << " dBm" << " Sensitivity: " << receiver->GetRxSensitivity());
		return;
	}

	//NS_LOG_INFO("invoking preamble reception");
	//NS_LOG_INFO("sensitiviy " << receiver->GetRxSensitivity());

	receiver->StartReceivePreamble(ppdu,
				       rxPowerDbm + receiver->GetRxGain());
}
void WurCommonChannel::Add(Ptr<WurCommonPhy> phy) {
	if (std::find(m_phyList.begin(), m_phyList.end(), phy) ==
	    m_phyList.end())
		m_phyList.push_back(phy);
}
}  // namespace ns3
