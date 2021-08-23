#include "wur-common-net-device-dummy-impl.h"
#include "ns3/log.h"
#include "ns3/mac8-address.h"
#include "ns3/node.h"
#include "ns3/wur-common-net-device-dummy-impl.h"
#include "ns3/wur-shared-mac-dummy-impl.h"
#include "ns3/wur-shared-mac.h"
#include "wur-common-phy.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/type-id.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurCommonNetDeviceDummyImpl");
bool WurCommonNetDeviceDummyImpl::SetMtu(const uint16_t mtu) {
	m_mtu = mtu <= m_maxMtu ? mtu : m_maxMtu;
	return mtu <= m_maxMtu ? true : false;
}
uint16_t WurCommonNetDeviceDummyImpl::GetMtu(void) const { 
	NS_LOG_FUNCTION(this << m_mtu);
	return m_mtu; 
}
bool WurCommonNetDeviceDummyImpl::IsLinkUp() const {
	return GetMainRadioPhy()->GetState() !=
	       WurCommonPhy::WurCommonPhyState::OFF;
}
bool WurCommonNetDeviceDummyImpl::IsBroadcast(void) const { return true; }
Address WurCommonNetDeviceDummyImpl::GetBroadcast(void) const {
	return Mac8Address::GetBroadcast();
}
bool WurCommonNetDeviceDummyImpl::IsMulticast(void) const { return false; }
Address WurCommonNetDeviceDummyImpl::GetMulticast(
    Ipv4Address multicastGroup) const {
	return Mac8Address::GetBroadcast();
}
Address WurCommonNetDeviceDummyImpl::GetMulticast(Ipv6Address addr) const {
	return Mac8Address::GetBroadcast();
}
bool WurCommonNetDeviceDummyImpl::IsBridge(void) const { return false; }
bool WurCommonNetDeviceDummyImpl::IsPointToPoint(void) const { return false; }
Ptr<Node> WurCommonNetDeviceDummyImpl::GetNode(void) const { return m_node; }
void WurCommonNetDeviceDummyImpl::SetNode(Ptr<Node> node) { m_node = node; }

bool WurCommonNetDeviceDummyImpl::SupportsSendFrom(void) const { return true; }

bool WurCommonNetDeviceDummyImpl::Send(Ptr<Packet> packet, const Address &dest,
				       uint16_t protocolNumber) {
	NS_LOG_FUNCTION(this << packet << dest << protocolNumber);
	return SendFrom(packet, GetAddress(), dest, protocolNumber);
}

bool WurCommonNetDeviceDummyImpl::SendFrom(Ptr<Packet> packet,
					   const Address &source,
					   const Address &dest,
					   uint16_t protocolNumber) {
	NS_LOG_FUNCTION(this << packet << source << dest << protocolNumber);
	NS_ASSERT(Mac8Address::IsMatchingType(dest));

	if (protocolNumber == 0) {
		// TODO: why not dest?
		Mac8Address realTo(Mac8Address::ConvertFrom(dest));
		Mac8Address realFrom(Mac8Address::ConvertFrom(source));
		NS_LOG_FUNCTION(this << realFrom << realTo);

		WurSharedMacDummyImpl::WurSharedMacDummyImplHeader macHeader;
		macHeader.SetTo(realTo);
		macHeader.SetFrom(realFrom);
		packet->AddHeader(macHeader);

		GetSharedMac()->NotifyTx(packet);
		GetSharedMac()->Enqueue(packet, realTo);
		return true;
	} else {
		return false;
	}
}
void WurCommonNetDeviceDummyImpl::AddLinkChangeCallback(
    Callback<void> callback) {}
bool WurCommonNetDeviceDummyImpl::NeedsArp(void) const { return false; }

TypeId WurCommonNetDeviceDummyImpl::GetTypeId() {
	static TypeId tid =
	    TypeId("ns3::WurCommonNetDeviceDummyImpl")
		.SetParent<NetDevice>()
		.AddConstructor<WurCommonNetDeviceDummyImpl>()
		.SetGroupName("Wur")
		//.AddAttribute(
		//    "Mtu", "The MAC-level Maximum Transmission Unit",
		//    UintegerValue(MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH),
		//    MakeUintegerAccessor(&WifiNetDevice::SetMtu,
		//                         &WifiNetDevice::GetMtu),
		//    MakeUintegerChecker<uint16_t>(
		//        1, MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH))
		.AddAttribute(
		    "Channel", "The channel attached to this device",
		    PointerValue(),
		    MakePointerAccessor(&WurCommonNetDevice::GetChannel),
		    MakePointerChecker<Channel>())
		.AddAttribute(
		    "Phy", "The PHY layer attached to this device.",
		    PointerValue(),
		    MakePointerAccessor(&WurCommonNetDevice::GetMainRadioPhy,
					&WurCommonNetDevice::SetMainRadioPhy),
		    MakePointerChecker<WurCommonPhy>())
		.AddAttribute(
		    "Mac", "The MAC layer attached to this device.",
		    PointerValue(),
		    MakePointerAccessor(&WurCommonNetDevice::GetSharedMac,
					&WurCommonNetDevice::SetSharedMac),
		    MakePointerChecker<WurSharedMac>());
	return tid;
}
}  // namespace ns3
