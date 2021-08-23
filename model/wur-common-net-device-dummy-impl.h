#ifndef WUR_COMMON_NET_DEVICE_DUMMY_IMPL_H
#define WUR_COMMON_NET_DEVICE_DUMMY_IMPL_H
#include "wur-common-net-device.h"
#include <bits/stdint-uintn.h>
namespace ns3 {
class WurCommonNetDeviceDummyImpl : public WurCommonNetDevice {
       public:
	bool SetMtu(const uint16_t mtu);
	virtual uint16_t GetMtu(void) const;
	bool IsLinkUp(void) const;
	void AddLinkChangeCallback(Callback<void> callback);
	bool IsBroadcast(void) const;
	Address GetBroadcast(void) const;
	bool IsMulticast(void) const;
	Address GetMulticast(Ipv4Address multicastGroup) const;
	Address GetMulticast(Ipv6Address addr) const;
	bool IsBridge(void) const;
	bool IsPointToPoint(void) const;
	bool Send(Ptr<Packet> packet, const Address& dest,
			  uint16_t protocolNumber);
	bool SendFrom(Ptr<Packet> packet, const Address& source,
			      const Address& dest, uint16_t protocolNumber);
	Ptr<Node> GetNode(void) const;
	void SetNode(Ptr<Node> node);
	bool NeedsArp(void) const;
	bool SupportsSendFrom(void) const;
	static TypeId GetTypeId(void);
       private:
	uint32_t m_mtu = 256;
	const uint32_t m_maxMtu = 256;
	Ptr<Node> m_node;
	ReceiveCallback m_rxCb;
	PromiscReceiveCallback m_promiscRxCb;
};
}  // namespace ns3

#endif /* WUR_COMMON_NET_DEVICE_DUMMY_IMPL_H */
