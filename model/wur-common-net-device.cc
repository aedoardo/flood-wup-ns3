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
void WurCommonNetDevice::SetPromiscReceiveCallback(
    PromiscReceiveCallback cb) {
	m_promiscRxCb = cb;
}
void WurCommonNetDevice::SetWakeUpSequence(std::string wakeUpSequence) {
	NS_LOG_DEBUG("Setting wake up sequence for device.");
	//std::cout << "Setting wake-up sequence" << std::endl;
	wakeUpSequence = wakeUpSequence;
}

std::string WurCommonNetDevice::GetWakeUpSequence() {
	return wakeUpSequence;
}

}  // namespace ns3
