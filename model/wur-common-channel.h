#ifndef WUR_COMMON_CHANNEL_H
#define WUR_COMMON_CHANNEL_H
#include "ns3/channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
//#include "wur-common-phy.h"
#include "wur-common-ppdu.h"
namespace ns3 {
class WurCommonPhy;
class WurCommonChannel : public Channel {
       public:
	static TypeId GetTypeId(void);
	WurCommonChannel() : Channel() {}
	virtual ~WurCommonChannel(){};
	void SetPropagationLossModel(const Ptr<PropagationLossModel> loss);
	void SetPropagationDelayModel(const Ptr<PropagationDelayModel> delay);
	void Send(Ptr<WurCommonPhy> sender, Ptr<const WurCommonPpdu> ppdu,
		  double txPowerDbm) const;
	void Add(Ptr<WurCommonPhy> phy);
	// inherited from channel
	virtual std::size_t GetNDevices(void) const;
	virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

       private:
	static void Receive(Ptr<WurCommonPhy> receiver, Ptr<WurCommonPpdu> ppdu,
			    double rxPowerDbm);
	typedef std::vector<Ptr<WurCommonPhy>> PhyList;
	PhyList m_phyList;  //!< List of WurMainRadioPhys connected to this
	Ptr<PropagationLossModel> m_loss;    //!< Propagation loss model
	Ptr<PropagationDelayModel> m_delay;  //!< Propagation delay model
};
}  // namespace ns3
#endif /* WUR_COMMON_CHANNEL_H */
