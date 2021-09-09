#include "../src/wake-up-radio/model/main-radio-energy-model.h"
#include "ns3/basic-energy-harvester.h"
#include "ns3/callback.h"
#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/simulator.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/mac8-address.h"
#include "ns3/node.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket.h"
#include "ns3/string.h"
#include "ns3/wur-common-channel.h"
#include "ns3/wur-shared-mac.h"
#include "ns3/wur-shared-mac-dummy-impl.h"
#include "ns3/wur-common-net-device.h"
#include "ns3/wur-common-net-device-dummy-impl.h"
#include "ns3/wur-common-phy.h"
#include "ns3/wur-common-phy-dummy-impl.h"
#include "ns3/wur-radio-energy-model.h"
#include "../src/energy/model/li-ion-energy-source.h"
#include "ns3/basic-energy-harvester-helper.h"
//
using namespace ns3;
//
int main(int argc, char** argv) {
	LogComponentEnable("WurCommonNetDevice", LOG_LEVEL_DEBUG);
    LogComponentEnable("WurSharedMac", LOG_LEVEL_ALL);
    LogComponentEnable("WurSharedMacDummyImpl", LOG_LEVEL_ALL);
	LogComponentEnable("WurCommonPhy", LOG_LEVEL_ALL);
	LogComponentEnable("WurCommonChannel", LOG_LEVEL_ALL);
	LogComponentEnable("FloodWUPPacketHeader", LOG_LEVEL_DEBUG);

	Ptr<Node> senderNode, receiverNode;

	Ptr<WurCommonNetDevice> senderDevice, receiverDevice;

	Ptr<WurCommonChannel> mainRadioChannel, wakeUpRadioChannel;

	Ptr<WurCommonPhy> receiverPhy, senderPhy;

	Ptr<WurCommonPhy> receiverWurPhy, senderWurPhy;

	Ptr<WurSharedMac> receiverMac, senderMac;

	Ptr<MobilityModel> senderMobility, receiverMobility;

	Ptr<PropagationLossModel> lossModel;

	Ptr<PropagationDelayModel> delayModel;


	senderDevice = CreateObject<WurCommonNetDeviceDummyImpl>();
	receiverDevice = CreateObject<WurCommonNetDeviceDummyImpl>();


	std::vector<Mac16Address> wakeUpSequenceList;
	Mac16Address t;
	for(int i = 0; i < 30; i++) {
		wakeUpSequenceList.push_back(t.Allocate());
	}
	//receiverDevice->SetWakeUpSequence(Mac16Address("00:01"));
	//senderDevice->SetWakeUpSequence(Mac16Address("00:00"));
	receiverDevice->SetWakeUpSequenceList(wakeUpSequenceList);
	senderDevice->SetWakeUpSequenceList(wakeUpSequenceList);

	senderNode = CreateObject<Node>();
	receiverNode = CreateObject<Node>();

	mainRadioChannel = CreateObject<WurCommonChannel>();
	wakeUpRadioChannel = CreateObject<WurCommonChannel>();

	senderPhy = CreateObject<WurCommonPhyDummyImpl>();
	receiverPhy = CreateObject<WurCommonPhyDummyImpl>();
	senderWurPhy = CreateObject<WurCommonPhyDummyImpl>();
	receiverWurPhy = CreateObject<WurCommonPhyDummyImpl>();

	senderMac = CreateObject<WurSharedMacDummyImpl>();
	receiverMac = CreateObject<WurSharedMacDummyImpl>();

	senderMobility = CreateObject<ConstantPositionMobilityModel>();
	receiverMobility = CreateObject<ConstantPositionMobilityModel>();

	senderMobility->SetPosition(Vector3D(0.0, 0.0, 0.0));
	receiverMobility->SetPosition(Vector3D(10.0, 10.0, 10.0));

	senderPhy->SetMobility(senderMobility);
	receiverPhy->SetMobility(receiverMobility);
	senderWurPhy->SetMobility(senderMobility);
	receiverWurPhy->SetMobility(receiverMobility);

	lossModel = CreateObject<FriisPropagationLossModel>();
	delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();
	mainRadioChannel->SetPropagationLossModel(lossModel);
	mainRadioChannel->SetPropagationDelayModel(delayModel);
	wakeUpRadioChannel->SetPropagationLossModel(lossModel);
	wakeUpRadioChannel->SetPropagationDelayModel(delayModel);

	senderPhy->SetTxGain(0);
	senderPhy->SetTxPower(20);

	receiverPhy->SetTxGain(0);
	senderPhy->SetRxGain(0);

	receiverPhy->SetRxGain(0);
	receiverPhy->SetRxSensitivity(-70);

	senderWurPhy->SetTxGain(0);
	senderWurPhy->SetTxPower(20);

	receiverWurPhy->SetRxGain(0);
	receiverWurPhy->SetRxSensitivity(-60);

	senderDevice->SetSharedMac(senderMac);
	receiverDevice->SetSharedMac(receiverMac);

	senderDevice->SetMainRadioPhy(senderPhy);
	receiverDevice->SetMainRadioPhy(receiverPhy);
    
	senderDevice->SetWurRadioPhy(senderWurPhy);
	receiverDevice->SetWurRadioPhy(receiverWurPhy);


	mainRadioChannel->Add(senderPhy);
	mainRadioChannel->Add(receiverPhy);
	wakeUpRadioChannel->Add(senderWurPhy);
	wakeUpRadioChannel->Add(receiverWurPhy);

	senderPhy->SetChannel(mainRadioChannel);
	receiverPhy->SetChannel(mainRadioChannel);
    
	senderMac->SetNetDevice(senderDevice);
	receiverMac->SetNetDevice(receiverDevice);
	senderPhy->SetDevice(senderDevice);
	receiverPhy->SetDevice(receiverDevice);
	senderWurPhy->SetDevice(senderDevice);
	receiverWurPhy->SetDevice(receiverDevice);

	senderWurPhy->SetChannel(wakeUpRadioChannel);
	receiverWurPhy->SetChannel(wakeUpRadioChannel);

	senderPhy->SetRxOkCallback(
	    MakeCallback(&WurSharedMac::OnDataRx, senderMac)
    );
	receiverPhy->SetRxOkCallback(
	    MakeCallback(&WurSharedMac::OnDataRx, receiverMac)
    );
	senderPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::NotifyTx, senderMac)
    );
	receiverPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::NotifyTx, receiverMac)
    );
	senderWurPhy->SetRxOkCallback(
        MakeCallback(&WurSharedMac::OnWurRx, senderMac)
    );
	senderWurPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::OnWurTx, senderMac)
    );
	receiverWurPhy->SetRxOkCallback(
        MakeCallback(&WurSharedMac::OnWurRx, receiverMac)
    );
	receiverWurPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::OnWurTx, receiverMac)
    );

	senderNode->AddDevice(senderDevice);
	receiverNode->AddDevice(receiverDevice);

	senderMac->SetAddress(Mac8Address(senderMac->GetNetDevice()->GetNode()->GetId()));
	receiverMac->SetAddress(Mac8Address(receiverMac->GetNetDevice()->GetNode()->GetId()));

	PacketSocketHelper pktskth;
	pktskth.Install(senderNode);
	PacketSocketAddress socket;
	socket.SetSingleDevice(senderNode->GetDevice(0)->GetIfIndex());
	socket.SetPhysicalAddress(receiverNode->GetDevice(0)->GetAddress());
    socket.SetProtocol(0);

	OnOffHelper onOffHelper("ns3::PacketSocketFactory", Address(socket));
	onOffHelper.SetConstantRate(DataRate(500), 32);
	onOffHelper.SetAttribute ("OnTime", 
        StringValue("ns3::ConstantRandomVariable[Constant=1]")
    ); 
	onOffHelper.SetAttribute("OffTime", 
        StringValue ("ns3::ConstantRandomVariable[Constant=0]")
    );

	ApplicationContainer apps = onOffHelper.Install(senderNode);
	apps.Start(Seconds(1.0));
	apps.Stop(Seconds(3.0));
	receiverPhy->TurnOff();
	senderPhy->TurnOff();
	senderWurPhy->TurnOn();
	receiverWurPhy->TurnOn();


	std::cout << "Starting simulation" << std::endl;
	Simulator::Stop(Seconds(10));
	Simulator::Run();
	Simulator::Destroy();
	std::cout << "End of simulation" << std::endl;
}
//
