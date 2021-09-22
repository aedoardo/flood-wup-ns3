#include "wur-shared-mac-dummy-impl.h"
#include "ns3/log-macros-enabled.h"
#include "ns3/mac8-address.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "wur-common-net-device.h"
#include "wur-common-phy.h"
#include "wur-common-psdu.h"
#include <string>
#include <iostream>
#include <sstream>
#include "flood-wakeup-packet.h"
#include "ns3/simulator.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurSharedMacDummyImpl");

void WurSharedMacDummyImpl::SetAddress(Address address) {
	m_address = Mac8Address::ConvertFrom(address);
}

Address WurSharedMacDummyImpl::GetAddress() const { return m_address; }

void WurSharedMacDummyImpl::StartWurTxMechanismImpl(Mac16Address wakeUpSequence) {
	NS_LOG_FUNCTION_NOARGS();
	Ptr<Packet> wurPacket = Create<Packet>();

	// Testing the new WakeUP packet.
	FloodWUPPacketHeader header;
	header.SetHeaderWakeUpSequence(wakeUpSequence);
	wurPacket->AddHeader(header);
	Ptr<WurCommonPsdu> psdu = Create<WurCommonPsdu>();
	psdu->SetPayload(wurPacket);
	psdu->SetType("wus");

	NS_LOG_FUNCTION(this << "WUR phy state" << GetWurRadioPhy()->GetState());
	

	if(GetWurRadioPhy()->GetState() == WurCommonPhy::WurCommonPhyState::IDLE) {
		GetWurRadioPhy()->StartTx(psdu);
		//waiting for TX ending
	}
}

void WurSharedMacDummyImpl::StartWurTxMechanismImpl() {
	NS_LOG_FUNCTION_NOARGS();
	Ptr<Packet> wurPacket = Create<Packet>();

	// Testing the new WakeUP packet.
	FloodWUPPacketHeader header;
	header.SetHeaderWakeUpSequence(m_netDevice->GetWakeUpSequence());
	wurPacket->AddHeader(header);
	Ptr<WurCommonPsdu> psdu = Create<WurCommonPsdu>();
	psdu->SetPayload(wurPacket);
	psdu->SetType("wus");

	NS_LOG_FUNCTION(this << "WUR phy state" << GetWurRadioPhy()->GetState());
	

	if(GetWurRadioPhy()->GetState() == WurCommonPhy::WurCommonPhyState::IDLE) {
		GetWurRadioPhy()->StartTx(psdu);
		//waiting for TX ending
	}
}

//to be set as txOkCallback in wur phy
void WurSharedMacDummyImpl::OnWurTx(Ptr<Packet> packet) {
	NS_LOG_FUNCTION_NOARGS();
	OnWurTxMechanismSuccess();	
}
void WurSharedMacDummyImpl::StartWurRxMechanismImpl() {
	// dummy invocation of OnWurRxMechanismSuccess
	NS_LOG_FUNCTION_NOARGS();
	OnWurRxMechanismSuccess();
}
void WurSharedMacDummyImpl::OnDataRx(Ptr<Packet> packet) {
	// TODO: print packet
	NS_LOG_FUNCTION_NOARGS();
	NS_LOG_DEBUG("Receiving data packet");
	WurSharedMacDummyImplHeader header;
       	packet->PeekHeader(header);	
	NS_LOG_FUNCTION(header.GetFrom() << " " << header.GetTo() << " ");
	if(Mac8Address::ConvertFrom(m_netDevice->GetAddress()) == header.GetTo() && m_netDevice->GetMainRadioPhy()->GetState() == WurCommonPhy::IDLE) {
		NS_LOG_DEBUG("Received data packet for device with wus: " << m_netDevice->GetWakeUpSequence() << "; pid: " << +header.id);
		// qui spegniamo la main radio e la wake up radio in IDLE del device.
		uint16_t pid = header.id;
		if(m_netDevice->GetLastPacketReceived().count(header.GetFrom()) == 0) {
			m_netDevice->lastPacketReceived.insert({header.GetFrom(), pid});
			m_netDevice->AdvanceWakeUpSequence(); // update the wake up sequence if it is a new packet and it is not a duplicate.
			m_netDevice->GetSharedMac()->m_forUpcb(packet, 0, header.GetFrom()); // forward to application
		} else {
			uint16_t lastId = m_netDevice->GetLastPacketReceived().find(header.GetFrom())->second;
			if(lastId == pid) {
					NS_LOG_DEBUG("Received packet is a duplicate!");
			} else {
					m_netDevice->lastPacketReceived.insert({header.GetFrom(), pid});
					NS_LOG_DEBUG("Received NEW packet from the sender, updated its value.");
					m_netDevice->AdvanceWakeUpSequence(); // update the wake up sequence if it is a new packet and it is not a duplicate.
					m_netDevice->GetSharedMac()->m_forUpcb(packet, 0, header.GetFrom()); // forward to application
			}
		}
		m_netDevice->GetMainRadioPhy()->TurnOff();
		//m_netDevice->GetWurRadioPhy()->TurnOn();
	} else {
		//Simulator::ScheduleNow(&WurSharedMac::Enqueue, this, packet, header.GetTo(), m_netDevice->GetWakeUpSequence());
		m_netDevice->GetSharedMac()->Enqueue(packet, header.GetTo(), m_netDevice->GetWakeUpSequence()); // flooding!
		m_netDevice->AdvanceWakeUpSequence(); // aggiorniamo la wake-up sequence.
		m_netDevice->GetMainRadioPhy()->TurnOff();
	}
}

//to be set as rxOkCallback in wur phy
void WurSharedMacDummyImpl::OnWurRx(Ptr<Packet> packet) {
	NS_LOG_FUNCTION_NOARGS();

	FloodWUPPacketHeader header;
	packet->PeekHeader(header);
	NS_LOG_DEBUG("Received a wake-up sequence packet with wus: " << header.GetWakeUpSequence());

	if(header.GetWakeUpSequence() == m_netDevice->GetWakeUpSequence()) {
			// qui riceviamo la wake up sequence per questo device
			m_netDevice->GetMainRadioPhy()->TurnOn(); // accendiamo la main radio
			NS_LOG_DEBUG("Turned on main radio.");
	}

}
void WurSharedMacDummyImpl::StartDataTx() {
	GetMainRadioPhy()->TurnOn();
	// check if phy is available
	if (GetMainRadioPhy()->GetState() ==
	    WurCommonPhy::WurCommonPhyState::IDLE) {

		NS_LOG_DEBUG("Starting data transmitting from device with wus " << m_netDevice->GetWakeUpSequence() << "!");

		Ptr<WurCommonPsdu> psdu = Create<WurCommonPsdu>();
		WurSharedMacDummyImplHeader header;
		std::pair<Ptr<Packet>,Address> item;

		item = m_txqueue.front();
		header.SetFrom(GetAddress());
		header.SetTo(std::get<1>(item)); //

		uint16_t pid = m_netDevice->GetNextPacketId();
		psdu->SetPacketId(pid);
		header.id = pid;
		Time creationTime = (Time) Simulator::Now().GetSeconds();

		NS_LOG_DEBUG("Creation time: " << creationTime);
		psdu->SetCreationTime(creationTime);

		Ptr<Packet> payload = std::get<0>(item);
		payload->AddHeader(header);
		psdu->SetPayload(payload);
		psdu->SetType("data");
		m_txqueue.erase(m_txqueue.begin());
		NS_LOG_FUNCTION("Starting transmitting packet with pid: " << pid << " and dst: " << header.GetTo());
		GetMainRadioPhy()->StartTx(psdu);


	}
}
void WurSharedMacDummyImpl::StartDataRx() { 
	NS_LOG_FUNCTION_NOARGS();
	GetMainRadioPhy()->TurnOn(); 
}
void WurSharedMacDummyImpl::SetPromisc(void) {}

TypeId WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetTypeId(void) {
	static TypeId tid =
	    TypeId("ns3::WurSharedMacDummyImpl::WurSharedMacDummyImplHeader")
		.SetParent<Chunk>()
		.SetGroupName("Wur");
	return tid;
}

TypeId WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetInstanceTypeId()
    const {
	return GetTypeId();
}

uint32_t WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetSerializedSize()
    const {
	return sizeof(uint8_t) * 2 + sizeof(uint16_t) * 1;
};

void WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Serialize(
    Buffer::Iterator start) const {
	uint8_t temp;
	m_from.CopyTo(&temp);
	start.WriteU8(temp);
	m_to.CopyTo(&temp);
	start.WriteU8(temp);
	start.WriteU16(id);
}
uint32_t WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Deserialize(
    Buffer::Iterator start) {
	Buffer::Iterator i = start;
	m_from = Mac8Address(i.ReadU8());
	m_to = Mac8Address(i.ReadU8());
	id = uint16_t (i.ReadU16());
	return i.GetDistanceFrom(start);
}

void WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Print(
    std::ostream &os) const {
	os << m_from << " " << m_to;
}

TypeId WurSharedMacDummyImpl::GetTypeId() {
	static TypeId tid = TypeId("ns3::WurSharedMacDummyImpl")
				.SetParent<WurSharedMac>()
				.AddConstructor<WurSharedMacDummyImpl>()
				.SetGroupName("Wur");
	//.AddAttribute(
	//    "Mtu", "The MAC-level Maximum Transmission Unit",
	//    UintegerValue(MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH),
	//    MakeUintegerAccessor(&WifiNetDevice::SetMtu,
	//                         &WifiNetDevice::GetMtu),
	//    MakeUintegerChecker<uint16_t>(
	//        1, MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH))
	return tid;
}

}  // namespace ns3
