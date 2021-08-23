//#include "wake-up-radio/model/main-radio-energy-model.h"
#include "./model/main-radio-energy-model.h"
#include "ns3/basic-energy-harvester.h"
#include "ns3/callback.h"
#include "ns3/device-energy-model.h"
#include "ns3/energy-source.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/simulator.h"
//#include "ns3/address.h"
//#include "ns3/application-container.h"
//#include "ns3/callback.h"
#include "ns3/constant-position-mobility-model.h"
//#include "ns3/data-rate.h"
//#include "ns3/inet-socket-address.h"
//#include "ns3/ipv4-address.h"
//#include "ns3/mac48-address.h"
#include "ns3/mac8-address.h"
//#include "ns3/net-device.h"
#include "ns3/node.h"
//#include "ns3/nstime.h"
//#include "ns3/object.h"
#include "ns3/on-off-helper.h"
//#include "ns3/packet-sink-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket.h"
//#include "ns3/propagation-delay-model.h"
//#include "ns3/propagation-loss-model.h"
//#include "ns3/socket.h"
#include "ns3/string.h"
//#include "ns3/vector.h"
//#include "ns3/wur-common-net-device.h"
//#include "ns3/udp-echo-helper.h"
#include "ns3/wur-common-channel.h"
#include "ns3/wur-shared-mac.h"
#include "ns3/wur-shared-mac-dummy-impl.h"
#include "ns3/wur-common-net-device.h"
#include "ns3/wur-common-net-device-dummy-impl.h"
#include "ns3/wur-common-phy.h"
#include "ns3/wur-common-phy-dummy-impl.h"
#include "ns3/wur-radio-energy-model.h"
#include "src/energy/model/li-ion-energy-source.h"
#include "ns3/basic-energy-harvester-helper.h"
//
using namespace ns3;
//
int main(int argc, char** argv) {
//	// variables definition
	Ptr<Node> senderNode, receiverNode;
	Ptr<WurCommonNetDevice> senderDevice, receiverDevice;
	Ptr<WurCommonChannel> mainRadioChannel;
	Ptr<WurCommonChannel> wakeUpRadioChannel;
	Ptr<WurCommonPhy> receiverPhy, senderPhy;
	Ptr<WurCommonPhy> receiverWurPhy, senderWurPhy;
	Ptr<WurSharedMac> receiverMac, senderMac;
	Ptr<MobilityModel> senderMobility, receiverMobility;
	Ptr<PropagationLossModel> lossModel;
	Ptr<PropagationDelayModel> delayModel;
	// obj creation
	senderDevice = CreateObject<WurCommonNetDeviceDummyImpl>();
	receiverDevice = CreateObject<WurCommonNetDeviceDummyImpl>();
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
//	//some customization
//	//in dB
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
	//receiverPhy->TurnOn();
//	// binding
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
	    MakeCallback(&WurSharedMac::OnDataRx, senderMac));
	receiverPhy->SetRxOkCallback(
	    MakeCallback(&WurSharedMac::OnDataRx, receiverMac));
	senderPhy->SetTxOkCallback(MakeCallback(&WurSharedMac::NotifyTx, senderMac));
	receiverPhy->SetTxOkCallback(MakeCallback(&WurSharedMac::NotifyTx, receiverMac));
	senderWurPhy->SetRxOkCallback(MakeCallback(&WurSharedMac::OnWurRx, senderMac));
	senderWurPhy->SetTxOkCallback(MakeCallback(&WurSharedMac::OnWurTx, senderMac));
	receiverWurPhy->SetRxOkCallback(MakeCallback(&WurSharedMac::OnWurRx, receiverMac));
	receiverWurPhy->SetTxOkCallback(MakeCallback(&WurSharedMac::OnWurTx, receiverMac));
	//TODO: set receiver wur callbacks
	senderNode->AddDevice(senderDevice);
	receiverNode->AddDevice(receiverDevice);
	senderMac->SetAddress(Mac8Address(senderMac->GetNetDevice()->GetNode()->GetId()));
	receiverMac->SetAddress(Mac8Address(receiverMac->GetNetDevice()->GetNode()->GetId()));
//	// apps
	PacketSocketHelper pktskth;
	pktskth.Install(senderNode);
	PacketSocketAddress socket;
	socket.SetSingleDevice(senderNode->GetDevice(0)->GetIfIndex());
	socket.SetPhysicalAddress(receiverNode->GetDevice(0)->GetAddress());
	//VIP: used in net device SendFrom/Send 
	socket.SetProtocol(0);
	OnOffHelper onOffHelper("ns3::PacketSocketFactory", Address(socket));
	onOffHelper.SetConstantRate(DataRate(500), 32);
	onOffHelper.SetAttribute ("OnTime", StringValue
	("ns3::ConstantRandomVariable[Constant=1]")); 
	onOffHelper.SetAttribute
	("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
	ApplicationContainer apps = onOffHelper.Install(senderNode);
	apps.Start(Seconds(1.0));
	apps.Stop(Seconds(9.0));
	receiverPhy->TurnOff();
	senderPhy->TurnOff();
	senderWurPhy->TurnOn();
	receiverWurPhy->TurnOn();

        /* Energy Section for sender */
        Ptr<EnergySource> energySource = CreateObject<LiIonEnergySource>();
        energySource->GetObject<LiIonEnergySource>()->SetInitialEnergy(0.14);
        Ptr<RadioEnergyModel> mainRadioEnergyModel = CreateObject<MainRadioEnergyModel>();
        mainRadioEnergyModel->SetEnergySource(energySource);
        mainRadioEnergyModel->SetNode(senderNode);
        Ptr<RadioEnergyModel> wurRadioEnergyModel = CreateObject<WurRadioEnergyModel>();
        wurRadioEnergyModel->SetEnergySource(energySource);
        wurRadioEnergyModel->SetNode(senderNode);


        energySource->AppendDeviceEnergyModel(mainRadioEnergyModel);
        energySource->AppendDeviceEnergyModel(wurRadioEnergyModel);
        energySource->SetNode(senderNode);
        BasicEnergyHarvesterHelper energyHelper;
        energyHelper.Install(energySource);
        senderPhy->SetEnergyModelCallback(MakeCallback(&DeviceEnergyModel::ChangeState,mainRadioEnergyModel));
        senderWurPhy->SetEnergyModelCallback(MakeCallback(&DeviceEnergyModel::ChangeState,wurRadioEnergyModel));


	std::cout << "Starting simulation" << std::endl;
	Simulator::Stop(Seconds(10));
	Simulator::Run();
	Simulator::Destroy();
}
//
