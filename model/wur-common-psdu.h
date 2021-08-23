#ifndef WUR_COMMON_PSDU_H
#define WUR_COMMON_PSDU_H
#include "ns3/packet.h"
#include "ns3/simple-ref-count.h"
namespace ns3 {
class WurCommonPsdu : public SimpleRefCount<WurCommonPsdu> {
       public:
	void SetPayload(Ptr<Packet> payload) { m_payload = payload; }
	Ptr<Packet> GetPayload() const { return m_payload; }

       private:
	Ptr<Packet> m_payload;
};
}  // namespace ns3
#endif /* WUR_COMMON_PSDU_H */
