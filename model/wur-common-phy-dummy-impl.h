#ifndef WUR_COMMON_PHY_DUMMY_IMPL_H
#define WUR_COMMON_PHY_DUMMY_IMPL_H

#include "wur-common-phy.h"
namespace ns3 {
class WurCommonPhyDummyImpl : public WurCommonPhy {
       public:
	void NotifyRxBegin(Ptr<const WurCommonPpdu>);
	void NotifyRxDrop(Ptr<const WurCommonPpdu>, std::string);
	void NotifyRxEnd(Ptr<const WurCommonPpdu>);
	void NotifyTxBegin(Ptr<const WurCommonPpdu>, double);
	void NotifyTxDrop(Ptr<const WurCommonPpdu>);
	void NotifyTxEnd(Ptr<const WurCommonPpdu>);
	void StartRx(Ptr<WurCommonPpdu> ppdu, double rxPowerDbm);
	void EndRx(Ptr<WurCommonPpdu> ppdu);
	void EndTx(Ptr<WurCommonPpdu> ppdu);
	void StartTx(Ptr<WurCommonPsdu> psdu);
	void SetDataRate(double dataRate);
       	double GetDataRate() const;	

       private:
	double m_dataRate = 1000000;
};

}  // namespace ns3

#endif /* WUR_COMMON_PHY_DUMMY_IMPL_H */
