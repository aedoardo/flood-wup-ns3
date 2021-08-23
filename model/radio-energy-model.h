#ifndef RADIO_ENERGY_MODEL_H
#define RADIO_ENERGY_MODEL_H
/**
 * Copied from AcousticModemEnergyModel implementation by Andrea Sacco
 */
#include "ns3/device-energy-model.h"
#include "ns3/traced-value.h"
#include "ns3/nstime.h"
namespace ns3 {
class RadioEnergyModel : public DeviceEnergyModel {
       public:
	/** Callback type for energy depletion handling. */
	typedef Callback<void> RadioEnergyDepletionCallback;

	/** Callback type for energy recharge handling. */
	typedef Callback<void> RadioEnergyRechargeCallback;

       public:
	/**
	 * Register this type.
	 * \return The object TypeId.
	 */
	static TypeId GetTypeId(void);
	/** Constructor. */
	RadioEnergyModel();
	/** Dummy destructor, see DoDispose */
	virtual ~RadioEnergyModel();

	/**
	 * Sets pointer to node.
	 *
	 * \param node Pointer to node.
	 */
	virtual void SetNode(Ptr<Node> node);

	/**
	 * Gets pointer to node.
	 *
	 * \return Pointer to node.
	 */
	virtual Ptr<Node> GetNode(void) const;

	// Inherited methods.
	virtual void SetEnergySource(Ptr<EnergySource> source);
	virtual double GetTotalEnergyConsumption(void) const;

	/**
	 * Get the transmission power of the modem.
	 *
	 * \return The transmission power in Watts.
	 */
	double GetTxPowerW(void) const;

	/**
	 * Set the transmission power of the modem.
	 *
	 * \param txPowerW Transmission power in watts.
	 */
	void SetTxPowerW(double txPowerW);

	/**
	 * Get the receiving power.
	 *
	 * \return The receiving power in Watts
	 */
	double GetRxPowerW(void) const;

	/**
	 * Set the receiving power of the modem.
	 *
	 * \param rxPowerW Receiving power in watts
	 */
	void SetRxPowerW(double rxPowerW);

	/**
	 *Get the idle power of the modem.
	 *
	 * \return The idle power in Watts
	 */
	double GetIdlePowerW(void) const;

	/**
	 * Set the idle state power of the modem.
	 *
	 * \param idlePowerW Idle power of the modem in watts.
	 */
	void SetIdlePowerW(double idlePowerW);

	/**
	 * Get the sleep state power of the modem.
	 *
	 * \return Sleep power of the modem in Watts
	 */
	double GetSleepPowerW(void) const;

	/**
	 * Set the sleep power of the modem.
	 *
	 * \param sleepPowerW Sleep power of the modem in watts.
	 */
	void SetSleepPowerW(double sleepPowerW);

	/**
	 * Get the current state of the modem.
	 *
	 * \return Current state.
	 */
	int GetCurrentState(void) const;

	/**
	 * \param callback Callback function.
	 *
	 * Sets callback for energy depletion handling.
	 */
	void SetEnergyDepletionCallback(RadioEnergyDepletionCallback callback);

	/**
	 * \param callback Callback function.
	 *
	 * Sets callback for energy recharge handling.
	 */
	void SetEnergyRechargeCallback(RadioEnergyRechargeCallback callback);

	/**
	 * Changes state of the RadioEnergyModel.
	 *
	 * \param newState New state the modem is in.
	 */
	virtual void ChangeState(int newState);

	/**
	 * \brief Handles energy depletion.
	 */
	virtual void HandleEnergyDepletion(void) = 0;

	/**
	 * \brief Handles energy recharged.
	 */
	virtual void HandleEnergyRecharged(void) = 0;

	/**
	 * \brief Handles energy changed.
	 *
	 * Not implemented
	 */
	virtual void HandleEnergyChanged(void);

       private:
	void DoDispose(void);

	/**
	 * \return Current draw of device, at current state.
	 */
	virtual double DoGetCurrentA(void) const;

	/**
	 * \param destState Modem state to switch to.
	 * \return True if the transition is allowed.
	 *
	 * This function checks if a given modem state transition is allowed.
	 */
	bool IsStateTransitionValid(const int destState);

	/**
	 * \param state New state the modem is currently in.
	 *
	 * Sets current state. This function is private so that only the energy
	 * model can change its own state.
	 */

       protected:
	Ptr<Node> m_node;  //!< The node hosting this transducer.
	RadioEnergyDepletionCallback m_energyDepletionCallback;
	RadioEnergyRechargeCallback m_energyRechargeCallback;
	void SetMicroModemState(const int state);
	int m_currentState;     //!< Current modem state.

       private:
	Ptr<EnergySource> m_source;  //!< The energy source.

	// Member variables for power consumption in different modem states.
	double m_txPowerW;     //!< The transmitter power, in watts.
	double m_rxPowerW;     //!< The receiver power, in watts.
	double m_idlePowerW;   //!< The idle power, in watts.
	double m_sleepPowerW;  //!< The sleep power, in watts.

	/** The total energy consumed by this model. */
	TracedValue<double> m_totalEnergyConsumption;

	// State variables.
	Time m_lastUpdateTime;  //!< Time stamp of previous energy update.


};  // namespace ns3

}  // namespace ns3

#endif /* RADIO_ENERGY_MODEL_H */
