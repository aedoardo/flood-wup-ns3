#ifndef WUR_COMMON_PHY_H
#define WUR_COMMON_PHY_H
#include "ns3/device-energy-model.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "wur-common-channel.h"
#include "wur-common-net-device.h"
namespace ns3 {
class WurCommonPpdu;
class WurCommonPsdu;
/**
 * \brief Class ancestor for both Main Radio and Wur Radio phy
 * It enforces the StartReceivePreamble where we check is the phy
 * can receive the packet and if the packet is truncated
 */ 
class WurCommonPhy : public Object {
       public:
	typedef Callback<void, Ptr<Packet> > RxOkCallback;
	typedef Callback<void, Ptr<Packet> > TxOkCallback; 
	typedef enum {
		OFF,
		IDLE,
		TX,
		RX,
		DISABLED,
	} WurCommonPhyState;
	Ptr<WurCommonChannel> GetChannel() const; 
	Ptr<MobilityModel> GetMobility() const; 
	Ptr<WurCommonNetDevice> GetDevice() const; 
	void SetDevice(Ptr<WurCommonNetDevice>);
	void SetMobility(Ptr<MobilityModel>);
	void SetChannel(Ptr<WurCommonChannel>);
	void StartReceivePreamble(Ptr<WurCommonPpdu> ppdu, double powerDbm);
	static TypeId GetTypeId(void);
	void SetRxGain(double rxGainDbm) { m_rxGainDbm = rxGainDbm; };
	void SetTxGain(double txGainDbm) { m_txGainDbm = txGainDbm; };
	void SetRxSensitivity(double rxSensitivityDbm) {
		m_rxSensitivityDbm = rxSensitivityDbm;
	};
	void SetTxPower(double txPowerDbm) { m_txPowerDbm = txPowerDbm; }
	double GetRxGain() const { return m_rxGainDbm; };
	double GetTxGain() const { return m_txGainDbm; };
	double GetRxSensitivity() const { return m_rxSensitivityDbm; };
	double GetTxPower() { return m_txPowerDbm; }
	WurCommonPhyState GetState() { return m_state; }
	void TurnOff();
	void TurnOn();
	virtual void NotifyRxBegin(Ptr<const WurCommonPpdu>) = 0;
	virtual void NotifyRxDrop(Ptr<const WurCommonPpdu>, std::string) = 0; 
	virtual void NotifyRxEnd(Ptr<const WurCommonPpdu>) = 0; 
	virtual void NotifyTxBegin(Ptr<const WurCommonPpdu>, double) = 0;
	virtual void NotifyTxDrop(Ptr<const WurCommonPpdu>) = 0;
	virtual void NotifyTxEnd(Ptr<const WurCommonPpdu>) = 0;
	virtual void StartRx(Ptr<WurCommonPpdu> ppdu, double rxPowerDbm) = 0;
	virtual void StartTx(Ptr<WurCommonPsdu> psdu) = 0;
        virtual void SetEnergyModelCallback(DeviceEnergyModel::ChangeStateCallback);
        virtual void EnergyDepletionHandler(void);
        virtual void EnergyRechargeHandler(void);
       private:
	Ptr<MobilityModel> m_mobility;
	Ptr<WurCommonPpdu> m_rxPacket;
	Ptr<WurCommonPpdu> m_txPacket;
	double m_rxGainDbm;
	double m_txGainDbm;
	double m_rxSensitivityDbm;
	double m_rxPowerDbm;
	double m_txPowerDbm;
        

       protected:
	Ptr<WurCommonChannel> m_channel;
	Ptr<WurCommonNetDevice> m_netDevice;
	RxOkCallback m_rxOkCallback;
	TxOkCallback m_txOkCallback;
        DeviceEnergyModel::ChangeStateCallback m_energyModelCallback;
	WurCommonPhyState m_state;
	Time m_preambleDuration;

	void SetRxPacket(Ptr<WurCommonPpdu> rxPacket) {
		m_rxPacket = rxPacket;
	}
	
	void SetTxPacket(Ptr<WurCommonPpdu> txPacket) {
		m_txPacket = txPacket;
	}

	Ptr<WurCommonPpdu> GetRxPacket() const { return m_rxPacket; }
	Ptr<WurCommonPpdu> GetTxPacket() const { return m_txPacket; }

	void UnsetRxPacket() { m_rxPacket = nullptr;  } 
	void UnsetTxPacket() { m_txPacket = nullptr;  } 

        void ChangeState(WurCommonPhy::WurCommonPhyState);

       public:
	void SetRxOkCallback(RxOkCallback callback) { m_rxOkCallback = callback; }
	void SetTxOkCallback(TxOkCallback callback) { m_txOkCallback = callback; }

};
}  // namespace ns3
#endif /* WUR_COMMON_PHY_H */
