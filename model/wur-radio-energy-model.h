#ifndef WUR_RADIO_ENERGY_MODEL_h
#define WUR_RADIO_ENERGY_MODEL_h

#include "radio-energy-model.h"

namespace ns3 {

class WurRadioEnergyModel : public RadioEnergyModel {
       public:
	/**
	 * \brief Handles energy depletion.
	 */
	virtual void HandleEnergyDepletion(void);

	/**
	 * \brief Handles energy recharged.
	 */
	virtual void HandleEnergyRecharged(void);
};

}  // namespace ns3

#endif
