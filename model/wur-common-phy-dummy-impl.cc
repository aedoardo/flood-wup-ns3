#include "wur-common-phy-dummy-impl.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "wur-common-phy.h"
#include "wur-common-ppdu.h"
#include "wur-shared-mac-dummy-impl.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("WurCommonPhyDummyImpl");
	void WurCommonPhyDummyImpl::NotifyRxBegin(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::NotifyRxDrop(Ptr<const WurCommonPpdu>,
						std::string) {}
	void WurCommonPhyDummyImpl::NotifyRxEnd(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::NotifyTxBegin(Ptr<const WurCommonPpdu>, double) {}
	void WurCommonPhyDummyImpl::NotifyTxDrop(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::NotifyTxEnd(Ptr<const WurCommonPpdu>) {}

	void WurCommonPhyDummyImpl::StartRx(Ptr<WurCommonPpdu> ppdu,
						double rxPowerDbm) {
		NS_LOG_FUNCTION_NOARGS();
		// just schedule EndRx
		Time delay = Seconds(((double)GetRxPacket()
					->GetPsdu()
					->GetPayload()
					->GetSerializedSize()) /
					m_dataRate);
		
		/*NS_LOG_FUNCTION(
			"data rate "
			<< m_dataRate << " payload size "
			<< GetRxPacket()->GetPsdu()->GetPayload()->GetSerializedSize()
			<< "rx delay " << delay);*/

		
		GetDevice()->GetSharedMac()->ReceiveWurPacket(ppdu->GetPsdu());
		//GetDevice()->GetSharedMac()->OnWurRx(ppdu);
		Simulator::Schedule(delay, &WurCommonPhyDummyImpl::EndRx, this, ppdu);
	}
	
	void WurCommonPhyDummyImpl::EndRx(Ptr<WurCommonPpdu> ppdu) {
		NS_LOG_FUNCTION_NOARGS();
		UnsetRxPacket();
		ChangeState(WurCommonPhy::IDLE);

		m_rxOkCallback(ppdu->GetPsdu()->GetPayload());
	}
	void WurCommonPhyDummyImpl::StartTx(Ptr<WurCommonPsdu> psdu) { 
		// assuming upper layer checked if idle
		NS_LOG_FUNCTION_NOARGS(); 

		Ptr<WurCommonPpdu> ppdu = Create<WurCommonPpdu>();
		ppdu->SetPsdu(psdu);

		SetTxPacket(ppdu);
		ChangeState(WurCommonPhy::TX); // qui cambia in transmitting

		Time delay = Seconds(
			(GetTxPacket()->GetPsdu()->GetPayload()->GetSerializedSize()) /
			m_dataRate);

		Simulator::Schedule(delay + m_preambleDuration,
					&WurCommonPhyDummyImpl::EndTx, this, ppdu);

		GetChannel()->Send(this, ppdu, GetTxPower());
	}
	void WurCommonPhyDummyImpl::EndTx(Ptr<WurCommonPpdu> ppdu) {
			NS_LOG_FUNCTION_NOARGS();
		UnsetTxPacket();
			//change state to IDLE, not to OFF (it is on behalf of the MAC protocol to do that)
			ChangeState(WurCommonPhy::IDLE);
		m_txOkCallback(ppdu->GetPsdu()->GetPayload());
	}

	void WurCommonPhyDummyImpl::SetDataRate(double dataRate) {
		m_dataRate = dataRate;
	}
	double WurCommonPhyDummyImpl::GetDataRate() const { return m_dataRate; }
}  // namespace ns3
