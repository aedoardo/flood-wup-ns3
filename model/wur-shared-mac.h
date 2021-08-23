#ifndef WUR_SHARED_MAC_H
#define WUR_SHARED_MAC_H
#include "ns3/address.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/timer.h"
#include "ns3/traced-callback.h"

namespace ns3 {
class WurCommonNetDevice;
class WurCommonPhy;
class WurSharedMac : public Object {
       public:
	// TODO: implement
	static TypeId GetTypeId();
	WurSharedMac() {
		Initialize();
	}
	virtual ~WurSharedMac() {}
	virtual Address GetAddress() const = 0;
	virtual void SetAddress(Address) = 0;
	void SetNetDevice(Ptr<WurCommonNetDevice>);
	Ptr<WurCommonNetDevice> GetNetDevice() const;
	Ptr<WurCommonPhy> GetMainRadioPhy() const;
	Ptr<WurCommonPhy> GetWurRadioPhy() const;

        virtual bool SupportsSendFrom(void) const = 0;
	//Enqueue packet from upper layer
        virtual void Enqueue(Ptr<Packet> packet, Address to);
	//Starting Wur mechanism for data transmission
	//Wrapper that starts wur tx timeout timer and invoke true implementation
        void StartWurTxMechanism();
	virtual void StartWurTxMechanismImpl() = 0;	
	//Starting Wur mechanism for data reception
        void StartWurRxMechanism();
        virtual void StartWurRxMechanismImpl() = 0;
	//Invoked when receiving packet from main radio
        virtual void OnDataRx(Ptr<Packet>) = 0;
	//Invoked when receiving packet from wur radio
        virtual void OnWurRx(Ptr<Packet>) = 0;
	//Invoked when phy successfully transmit a wur packet
        virtual void OnWurTx(Ptr<Packet>) = 0;
	//Timeout timer for wur tx mechanism
        void TimerWurTxMechanismCallback();
	//Timeout timer for wur tx mechanism
        void TimerWurRxMechanismCallback();
	//Timeout for main radio data reception
        void TimerDataReceivingCallback();
	//Timeout for main radio data transmission
        void TimerDataSendingCallback();
	//Wrapper for main radio data transmission (start main radio tx timer)
        void StartDataTxWrapper();
	//True data transmission
        virtual void StartDataTx() = 0;
	void StartDataRxWrapper();
	//Start main radio reception
        virtual void StartDataRx() = 0;
	//Invoked when wur tx mechanism succeed
        void OnWurTxMechanismSuccess();
	//Invoked when wur rx mechanism succeed
	void OnWurRxMechanismSuccess();
	//Notification of successful data transmission
        void NotifyTx(Ptr<Packet>);
        virtual void SetPromisc(void) = 0;

        typedef enum {
                IDLE,
                WUR_RX_MECHANISM,
		WUR_TX_MECHANISM,
                SENDING_DATA,
                RECEIVING_DATA,
        } WurSharedMacState;

       protected: 
	Ptr<WurCommonNetDevice> m_netDevice;
	WurSharedMacState m_state;
        std::vector<std::pair<Ptr<Packet>,Address>> m_txqueue;
       private:
        Timer wurSendingTimer;
	Timer wurReceivingTimer;
        Timer dataSendingTimer;
        Timer dataReceivingTimer;
        Time WUR_TX_MECHANISM_TIMEOUT = MilliSeconds(20);
        Time WUR_RX_MECHANISM_TIMEOUT = MilliSeconds(10);
        Time DATA_RECEPTION_TIMEOUT = MilliSeconds(50);
        Time DATA_TRANSMISSION_TIMEOUT = MilliSeconds(30);
        TracedCallback<Ptr<const Packet>> m_macTxTrace;
        TracedCallback<Ptr<const Packet>> m_macTxDropTrace;
        TracedCallback<Ptr<const Packet>> m_macRxDropTrace;
        TracedCallback<Ptr<const Packet>> m_macPromiscRxTrace;
        TracedCallback<Ptr<const Packet>> m_macRxTrace;
        void Initialize();
};

}  // namespace ns3

#endif /* WUR_SHARED_MAC_H */
