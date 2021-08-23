#ifndef WUR_COMMON_PPDU_H
#define WUR_COMMON_PPDU_H
#include "ns3/simple-ref-count.h"
#include "wur-common-psdu.h"
namespace ns3 {
	class WurCommonPpdu : public SimpleRefCount<WurCommonPpdu> {
		public:
			bool IsTruncatedTx() { return m_isTruncatedTx; }
			void SetTruncatedTx() { m_isTruncatedTx = true; }
			bool IsTruncatedRx() { return m_isTruncatedRx; }
			void SetTruncatedRx() { m_isTruncatedRx = true; }
			void SetPsdu(Ptr<WurCommonPsdu> psdu) { m_psdu = psdu; }
			Ptr<WurCommonPsdu> GetPsdu() const { return m_psdu; }
		private:
			bool m_isTruncatedTx = false;
			bool m_isTruncatedRx = false;
			Ptr<WurCommonPsdu> m_psdu;
	};
}
#endif /* WUR_COMMON_PPDU_H */
