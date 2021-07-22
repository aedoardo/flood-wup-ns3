#ifndef FLOOD_WUP_CHANNEL
#define FLOOD_WUP_CHANNEL

#include "ns3/channel.h"

namespace ns3 {

    class PropagationDelayModel;

    class FloodWUPChannel : public Channel {

        public:
            static TypeId GetTypeId(void);

            FloodWUPChannel();
            virtual ~FloodWUPChannel();

            virtual std::size_t GetNDevices(void) const;

            virtual Ptr<NetDevice> GetDevice(std::size_t i) const; // TODO: al momento NetDevice.. da vedere come fare FloodWUPMain

            void Add(); // TODO: per il PHY
            void SetPropagation(const Ptr<PropagationDelayModel> delay); // TODO: vedere PropagationDelayModel.

            void Send(); // TODO: parametri PHY, ppdu, txPowerDbm.

            int64_t AssignStreams(int64_t stream);

        private:
            void Receive(); // TODO: come per il metodo Send.

            Ptr<PropagationDelayModel> m_delay;

    };

}


#endif