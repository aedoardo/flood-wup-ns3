#ifndef WUR_COMMON_PSDU_H
#define WUR_COMMON_PSDU_H
#include "ns3/packet.h"
#include "ns3/simple-ref-count.h"
namespace ns3 {
class WurCommonPsdu : public SimpleRefCount<WurCommonPsdu> {
       public:
	void SetPayload(Ptr<Packet> payload) { m_payload = payload; }
	Ptr<Packet> GetPayload() const { return m_payload; }
	std::string GetPacketType() {return m_type; };
	void SetType(std::string s) {m_type = s;};
	void SetPacketId(uint16_t t) {m_pid = t;};
	uint16_t GetPacketId() {return m_pid;};
	
       private:
	Ptr<Packet> m_payload;
	std::string m_type;
	uint16_t m_pid;
};
}  // namespace ns3
#endif /* WUR_COMMON_PSDU_H */
