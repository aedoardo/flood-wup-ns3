#ifndef WUR_COMMON_NET_DEVICE_H
#define WUR_COMMON_NET_DEVICE_H
#include <bits/stdint-uintn.h>
#include "ns3/net-device.h"
#include "wur-common-channel.h"
#include "string.h"
#include "ns3/mac16-address.h"
#include "ns3/mac8-address.h"
//#include "wur-common-phy.h"



namespace ns3 {

	struct PacketReceived {
		Mac8Address addr;
		uint16_t packetId;

		bool operator==(PacketReceived& p) const {
			return addr == p.addr && packetId == p.packetId;
		};
	};

	class WurSharedMac;
	class WurCommonPhy;

	class WurCommonNetDevice : public NetDevice {
		private:
			uint32_t m_ifIndex;
			Ptr<WurSharedMac> m_mac;
			Ptr<WurCommonPhy> m_mainRadioPhy;
			Ptr<WurCommonPhy> m_wurRadioPhy;
			ReceiveCallback m_rxCb;
			PromiscReceiveCallback m_promiscRxCb;
			Mac16Address wakeUpSequence;
			std::map<PacketReceived, Time> cachePacketId;
			uint16_t currentPacketId = 0;

		public:
		
			void SetWakeUpSequence(Mac16Address wakeUp);
			uint16_t GetNextPacketId();
			Mac16Address GetWakeUpSequence();
			Ptr<WurCommonPhy> GetMainRadioPhy() const;
			Ptr<WurCommonPhy> GetWurRadioPhy() const;
			Ptr<WurSharedMac> GetSharedMac() const; 
			void SetMainRadioPhy(Ptr<WurCommonPhy> mainRadioPhy);
			void SetWurRadioPhy(Ptr<WurCommonPhy> wurRadioPhy);
			void SetSharedMac(Ptr<WurSharedMac> mac); 
			Ptr<WurCommonChannel> GetMainRadioChannel() const;
			Ptr<WurCommonChannel> GetWurRadioChannel() const;
		// inherithed from NetDevice
		public:
			/**
			 * \brief Get the type ID.
			 * \return the object TypeId
			 */
			//static TypeId GetTypeId(void);

			/**
			 * \param index ifIndex of the device
			 */
			virtual void SetIfIndex(const uint32_t index) { m_ifIndex = index; }
			/**
			 * \return index ifIndex of the device
			 */
			virtual uint32_t GetIfIndex(void) const { return m_ifIndex; }

			/**
			 * \return the channel this NetDevice is connected to. It is the Main
			 * Radio channel. The value returned can be zero if the NetDevice is not
			 * yet connected to any channel or if the underlying NetDevice has no
			 *         concept of a channel. i.e., callers _must_ check for zero
			 *         and be ready to handle it.
			 */
			virtual Ptr<Channel> GetChannel(void) const; 
			/**
			 * Set the address of this interface
			 * \param address address to set
			 */
			virtual void SetAddress(Address address);

			/**
			 * \return the current Address of this interface.
			 */
			virtual Address GetAddress(void) const;

			/**
			 * \param mtu MTU value, in bytes, to set for the device
			 * \return whether the MTU value was within legal bounds
			 *
			 * Override for default MTU defined on a per-type basis.
			 */
			virtual bool SetMtu(const uint16_t mtu) = 0;
			/**
			 * \return the link-level MTU in bytes for this interface.
			 *
			 * This value is typically used by the IP layer to perform
			 * IP fragmentation when needed.
			 */
			virtual uint16_t GetMtu(void) const = 0;
			/**
			 * \return true if link is up; false otherwise
			 */
			virtual bool IsLinkUp(void) const = 0;
			/**
			 * TracedCallback signature for link changed event.
			 */
			typedef void (*LinkChangeTracedCallback)(void);
			/**
			 * \param callback the callback to invoke
			 *
			 * Add a callback invoked whenever the link
			 * status changes to UP. This callback is typically used
			 * by the IP/ARP layer to flush the ARP cache and by IPv6 stack
			 * to flush NDISC cache whenever the link goes up.
			 */
			virtual void AddLinkChangeCallback(Callback<void> callback) = 0;
			/**
			 * \return true if this interface supports a broadcast address,
			 *         false otherwise.
			 */
			virtual bool IsBroadcast(void) const = 0;
			/**
			 * \return the broadcast address supported by
			 *         this netdevice.
			 *
			 * Calling this method is invalid if IsBroadcast returns
			 * not true.
			 */
			virtual Address GetBroadcast(void) const = 0;

			/**
			 * \return value of m_isMulticast flag
			 */
			virtual bool IsMulticast(void) const = 0;

			/**
			 * \brief Make and return a MAC multicast address using the provided
			 *        multicast group
			 *
			 * \RFC{1112} says that an Ipv4 host group address is mapped to an
			 * Ethernet multicast address by placing the low-order 23-bits of the IP
			 * address into the low-order 23 bits of the Ethernet multicast address
			 * 01-00-5E-00-00-00 (hex).  Similar RFCs exist for Ipv6 and Eui64
			 * mappings. This method performs the multicast address creation
			 * function appropriate to the underlying MAC address of the device.
			 * This MAC address is encapsulated in an abstract Address to avoid
			 * dependencies on the exact MAC address format.
			 *
			 * In the case of net devices that do not support
			 * multicast, clients are expected to test NetDevice::IsMulticast and
			 * avoid attempting to map multicast packets.  Subclasses of NetDevice
			 * that do support multicasting are expected to override this method and
			 * provide an implementation appropriate to the particular device.
			 *
			 * \param multicastGroup The IP address for the multicast group
			 * destination of the packet. \return The MAC multicast Address used to
			 * send packets to the provided multicast group.
			 *
			 * \warning Calling this method is invalid if IsMulticast returns not
			 * true. \see IsMulticast()
			 */
			virtual Address GetMulticast(Ipv4Address multicastGroup) const = 0;

			/**
			 * \brief Get the MAC multicast address corresponding
			 * to the IPv6 address provided.
			 * \param addr IPv6 address
			 * \return the MAC multicast address
			 * \warning Calling this method is invalid if IsMulticast returns not
			 * true.
			 */
			virtual Address GetMulticast(Ipv6Address addr) const = 0;

			/**
			 * \brief Return true if the net device is acting as a bridge.
			 *
			 * \return value of m_isBridge flag
			 */
			virtual bool IsBridge(void) const = 0;

			/**
			 * \brief Return true if the net device is on a point-to-point link.
			 *
			 * \return value of m_isPointToPoint flag
			 */
			virtual bool IsPointToPoint(void) const = 0;
			/**
			 * \param packet packet sent from above down to Network Device
			 * \param dest mac address of the destination (already resolved)
			 * \param protocolNumber identifies the type of payload contained in
			 *        this packet. Used to call the right L3Protocol when the packet
			 *        is received.
			 *
			 *  Called from higher layer to send packet into Network Device
			 *  to the specified destination Address
			 *
			 * \return whether the Send operation succeeded
			 */
			virtual bool Send(Ptr<Packet> packet, const Address& dest,
					uint16_t protocolNumber) = 0;
			/**
			 * \param packet packet sent from above down to Network Device
			 * \param source source mac address (so called "MAC spoofing")
			 * \param dest mac address of the destination (already resolved)
			 * \param protocolNumber identifies the type of payload contained in
			 *        this packet. Used to call the right L3Protocol when the packet
			 *        is received.
			 *
			 *  Called from higher layer to send packet into Network Device
			 *  with the specified source and destination Addresses.
			 *
			 * \return whether the Send operation succeeded
			 */
			virtual bool SendFrom(Ptr<Packet> packet, const Address& source,
						const Address& dest, uint16_t protocolNumber) = 0;
			/**
			 * \returns the node base class which contains this network
			 *          interface.
			 *
			 * When a subclass needs to get access to the underlying node
			 * base class to print the nodeid for example, it can invoke
			 * this method.
			 */
			virtual Ptr<Node> GetNode(void) const = 0;

			/**
			 * \param node the node associated to this netdevice.
			 *
			 * This method is called from ns3::Node::AddDevice.
			 */
			virtual void SetNode(Ptr<Node> node) = 0;

			/**
			 * \returns true if ARP is needed, false otherwise.
			 *
			 * Called by higher-layers to check if this NetDevice requires
			 * ARP to be used.
			 */
			virtual bool NeedsArp(void) const = 0;

			/**
			 * Packet types are used as they are in Linux.  GCC name resolution on
			 * typedef enum {} PacketType is broken for the foreseeable future, so
			 * if you need to use ns-3 PacketType in a driver that also uses the
			 * Linux packet types you're hosed unless we define a shadow type,
			 * which we do here.
			 */
			enum PacketType {
				PACKET_HOST = 1, /**< Packet addressed oo us */
				NS3_PACKET_HOST = PACKET_HOST,
				PACKET_BROADCAST, /**< Packet addressed to all */
				NS3_PACKET_BROADCAST = PACKET_BROADCAST,
				PACKET_MULTICAST, /**< Packet addressed to multicast group */
				NS3_PACKET_MULTICAST = PACKET_MULTICAST,
				PACKET_OTHERHOST, /**< Packet addressed to someone else */
				NS3_PACKET_OTHERHOST = PACKET_OTHERHOST,
			};

			/**
			 * \param device a pointer to the net device which is calling this
			 * callback \param packet the packet received \param protocol the 16 bit
			 * protocol number associated with this packet. This protocol number is
			 * expected to be the same protocol number given to the Send method by
			 * the user on the sender side. \param sender the address of the sender
			 * \returns true if the callback could handle the packet successfully,
			 * false otherwise.
			 */

			/**
			 * \param cb callback to invoke whenever a packet has been received and
			 * must be forwarded to the higher layers.
			 *
			 * Set the callback to be used to notify higher layers when a packet has
			 * been received.
			 */
			virtual void SetReceiveCallback(ReceiveCallback cb);

			virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
			/**
			 * \param device a pointer to the net device which is calling this
			 * callback \param packet the packet received \param protocol the 16 bit
			 * protocol number associated with this packet. This protocol number is
			 * expected to be the same protocol number given to the Send method by
			 * the user on the sender side. \param sender the address of the sender
			 * \param receiver the address of the receiver
			 * \param packetType type of packet received
			 * (broadcast/multicast/unicast/otherhost) \returns true if the callback
			 * could handle the packet successfully, false otherwise.
			 */

			/**
			 * \param cb callback to invoke whenever a packet has been received in
			 * promiscuous mode and must be forwarded to the higher layers.
			 *
			 * Enables netdevice promiscuous mode and sets the callback that
			 * will handle promiscuous mode packets.  Note, promiscuous mode
			 * packets means _all_ packets, including those packets that can be
			 * sensed by the netdevice but which are intended to be received by
			 * other hosts.
			 */

			/**
			 * \return true if this interface supports a bridging mode, false
			 * otherwise.
			 */
			virtual bool SupportsSendFrom(void) const = 0;
	};
}  // namespace ns3
#endif /* WUR_COMMON_NET_DEVICE_H */
