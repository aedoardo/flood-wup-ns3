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

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurSharedMacDummyImpl");
void WurSharedMacDummyImpl::SetAddress(Address address) {
	m_address = Mac8Address::ConvertFrom(address);
}
Address WurSharedMacDummyImpl::GetAddress() const { return m_address; }
void WurSharedMacDummyImpl::StartWurTxMechanismImpl() {
	NS_LOG_FUNCTION_NOARGS();
	Ptr<Packet> wurPacket = Create<Packet>();

	WurSharedMacDummyImplHeader header;
	header.SetFrom(GetAddress());
	header.SetTo(std::get<1>(m_txqueue.front()));
	header.SetPacketType(WurPacketType::WakeUpPacket);
	header.SetHeaderWakeUpSequence("567891");
	wurPacket->AddHeader(header);

	Ptr<WurCommonPsdu> psdu = Create<WurCommonPsdu>();
	psdu->SetPayload(wurPacket);
	NS_LOG_FUNCTION(this << "WUR phy state" << GetWurRadioPhy()->GetState());
	

	if(GetWurRadioPhy()->GetState() == WurCommonPhy::WurCommonPhyState::IDLE) {
		NS_LOG_FUNCTION(this << "Sending WUR data packet.");

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
	WurSharedMacDummyImplHeader header;
       	packet->PeekHeader(header);	
	NS_LOG_FUNCTION(header.GetFrom() << " " << header.GetTo());
}

//to be set as rxOkCallback in wur phy
void WurSharedMacDummyImpl::OnWurRx(Ptr<Packet> packet) {
	NS_LOG_FUNCTION_NOARGS();
	WurSharedMacDummyImplHeader header;
	packet->RemoveHeader(header);
	
	
	if(header.GetTo() == Mac8Address::ConvertFrom(GetAddress())) {
		//if IDLE, start wur rx mechanism
		if(m_state  == WurSharedMac::WurSharedMacState::IDLE) {
			
			if(header.GetPacketType() == WurPacketType::WakeUpPacket)
				NS_LOG_FUNCTION("Received WUR packet with sequence: " + header.GetWakeUpSequenceHeader());
			else
				NS_LOG_FUNCTION("Received WUR packet data.");
			
			//NS_LOG_FUNCTION("Received WUR packet for me " << header.GetTo());
			StartWurRxMechanism();
		}
			
	}	
}
void WurSharedMacDummyImpl::StartDataTx() {
	NS_LOG_FUNCTION_NOARGS();
	GetMainRadioPhy()->TurnOn();
	// check if phy is available
	if (GetMainRadioPhy()->GetState() ==
	    WurCommonPhy::WurCommonPhyState::IDLE) {
		Ptr<WurCommonPsdu> psdu = Create<WurCommonPsdu>();
		WurSharedMacDummyImplHeader header;
		std::pair<Ptr<Packet>,Address> item;
		item = m_txqueue.front();
		header.SetFrom(GetAddress());
		header.SetTo(std::get<1>(item));
		Ptr<Packet> payload = std::get<0>(item);
		payload->AddHeader(header);
		psdu->SetPayload(payload);
		m_txqueue.erase(m_txqueue.begin());
		NS_LOG_FUNCTION("Starting transmitting packet");
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
	return sizeof(uint8_t) * 3 + sizeof(uint64_t) * 1;
};

void WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Serialize(
    Buffer::Iterator start) const {
	uint8_t temp;
	m_from.CopyTo(&temp);
	start.WriteU8(temp);
	m_to.CopyTo(&temp);
	start.WriteU8(temp);
	if(m_type == WurPacketType::WakeUpPacket) {
		start.WriteU8(0);
		start.WriteU64(atoi(m_seq.c_str()));
	} else {
		start.WriteU8(1);
		start.WriteU64(0);
	}
}
uint32_t WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Deserialize(
    Buffer::Iterator start) {
	Buffer::Iterator i = start;
	m_from = Mac8Address(i.ReadU8());
	m_to = Mac8Address(i.ReadU8());
	m_type = (WurPacketType) i.ReadU8();
	m_seq = std::to_string(i.ReadU64());
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
