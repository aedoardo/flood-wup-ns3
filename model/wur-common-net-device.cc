#include "wur-common-net-device.h"
#include "ns3/channel.h"
#include "wur-shared-mac.h"
#include "wur-common-phy.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("WurCommonNetDevice");
	NS_OBJECT_ENSURE_REGISTERED(WurCommonNetDevice);
void WurCommonNetDevice::SetAddress(Address address) {
	m_mac->SetAddress(address);
}
Address WurCommonNetDevice::GetAddress() const { return m_mac->GetAddress(); }
Ptr<WurCommonPhy> WurCommonNetDevice::GetMainRadioPhy() const {
	return m_mainRadioPhy;
}
Ptr<WurCommonPhy> WurCommonNetDevice::GetWurRadioPhy() const {
	return m_wurRadioPhy;
}
Ptr<WurSharedMac> WurCommonNetDevice::GetSharedMac() const { return m_mac; }
void WurCommonNetDevice::SetMainRadioPhy(Ptr<WurCommonPhy> mainRadioPhy) {
	m_mainRadioPhy = mainRadioPhy;
}
void WurCommonNetDevice::SetWurRadioPhy(Ptr<WurCommonPhy> wurRadioPhy) {
	m_wurRadioPhy = wurRadioPhy;
}
void WurCommonNetDevice::SetSharedMac(Ptr<WurSharedMac> mac) { m_mac = mac; }
Ptr<WurCommonChannel> WurCommonNetDevice::GetMainRadioChannel() const {
	return m_mainRadioPhy->GetChannel();
}
Ptr<WurCommonChannel> WurCommonNetDevice::GetWurRadioChannel() const {
	return m_wurRadioPhy->GetChannel();
}
Ptr<Channel> WurCommonNetDevice::GetChannel() const {
	return m_mainRadioPhy->GetChannel();
}

void WurCommonNetDevice::SetReceiveCallback(ReceiveCallback cb) {
	m_rxCb = cb;
}

void WurCommonNetDevice::ForwardUp(Ptr<Packet> pkt, uint16_t n, const Mac8Address& addr) {
	NS_LOG_DEBUG("RECEIVED PACKET AT APPLICATION \n \n \n \n");
}

void WurCommonNetDevice::SetPromiscReceiveCallback(
    PromiscReceiveCallback cb) {
	m_promiscRxCb = cb;
}
void WurCommonNetDevice::SetWakeUpSequence(Mac16Address _wkupseq) {
	NS_LOG_DEBUG("Setting "<< _wkupseq <<" as wake up sequence for device.");
	wakeUpSequence = _wkupseq;
}

Mac16Address WurCommonNetDevice::GetWakeUpSequence() {
	return wakeUpSequence;
}

uint16_t WurCommonNetDevice::GetNextPacketId() {
	currentPacketId += 1;
	uint16_t toRet = currentPacketId;
	return toRet;
}

}  // namespace ns3
