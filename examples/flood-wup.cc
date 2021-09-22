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
#include <random>
#include <iostream>
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
 
    Ptr<Node> senderNode;
    Ptr<WurCommonNetDevice> senderDevice;
    Ptr<WurCommonChannel> mainRadioChannel, wakeUpRadioChannel;
    Ptr<WurCommonPhy> senderPhy;
    Ptr<WurCommonPhy> senderWurPhy;
    Ptr<MobilityModel> senderMobility;
    Ptr<WurSharedMac> senderMac;
    Ptr<PropagationLossModel> lossModel;
    Ptr<PropagationDelayModel> delayModel;
 
    senderDevice = CreateObject<WurCommonNetDeviceDummyImpl>();
 
    std::vector<Mac16Address> wakeUpSequenceList;
    Mac16Address t;
    for(int i = 0; i < 30; i++) {
        wakeUpSequenceList.push_back(t.Allocate());
    }
 
    senderDevice->SetWakeUpSequenceList(wakeUpSequenceList);
    senderNode = CreateObject<Node>();
 
    mainRadioChannel = CreateObject<WurCommonChannel>();
    wakeUpRadioChannel = CreateObject<WurCommonChannel>();
 
    senderPhy = CreateObject<WurCommonPhyDummyImpl>();
    senderWurPhy = CreateObject<WurCommonPhyDummyImpl>();
 
    senderMac = CreateObject<WurSharedMacDummyImpl>();
    senderMobility = CreateObject<ConstantPositionMobilityModel>();
 
    senderMobility->SetPosition(Vector3D(0.0, 0.0, 0.0));
    senderPhy->SetMobility(senderMobility);
    senderWurPhy->SetMobility(senderMobility);
 
    lossModel = CreateObject<FriisPropagationLossModel>();
    delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();
 
    mainRadioChannel->SetPropagationLossModel(lossModel);
    mainRadioChannel->SetPropagationDelayModel(delayModel);
 
    wakeUpRadioChannel->SetPropagationLossModel(lossModel);
    wakeUpRadioChannel->SetPropagationDelayModel(delayModel);
 
    senderPhy->SetTxGain(0);
    senderPhy->SetTxPower(20);
    senderPhy->SetRxSensitivity(-83);
 
    senderPhy->SetRxGain(0);
    senderWurPhy->SetTxGain(0);
    senderWurPhy->SetRxSensitivity(-83);
 
    senderWurPhy->SetTxPower(20);
    senderDevice->SetSharedMac(senderMac);
    senderDevice->SetMainRadioPhy(senderPhy);
    senderDevice->SetWurRadioPhy(senderWurPhy);
 
    mainRadioChannel->Add(senderPhy);
    wakeUpRadioChannel->Add(senderWurPhy);
 
    senderPhy->SetChannel(mainRadioChannel);
    senderMac->SetNetDevice(senderDevice);
 
    senderPhy->SetDevice(senderDevice);
    senderWurPhy->SetDevice(senderDevice);
 
    senderWurPhy->SetChannel(wakeUpRadioChannel);
    
    senderPhy->SetRxOkCallback(
        MakeCallback(&WurSharedMac::OnDataRx, senderMac)
    );
    senderPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::NotifyTx, senderMac)
    );
    senderWurPhy->SetRxOkCallback(
        MakeCallback(&WurSharedMac::OnWurRx, senderMac)
    );
    senderWurPhy->SetTxOkCallback(
        MakeCallback(&WurSharedMac::OnWurTx, senderMac)
    );
    senderNode->AddDevice(senderDevice);
    senderMac->SetAddress(Mac8Address(senderMac->GetNetDevice()->GetNode()->GetId()));
 
    std::vector<Ptr<Node>> nodes;
    std::vector<Ptr<WurCommonPhy>> phyNodes;
    std::vector<Ptr<WurCommonPhy>> wurPhyNodes;
    std::vector<Ptr<WurCommonNetDevice>> nodesDevice;
    std::vector<Ptr<MobilityModel>> nodesMobility;
    std::vector<Ptr<WurSharedMac>> nodesMac;
 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 50);
 
 
    PacketSocketHelper pktskth;
    pktskth.Install(senderNode);
    PacketSocketAddress socket;
    socket.SetSingleDevice(senderNode->GetDevice(0)->GetIfIndex());
 
 
    int numberOfDevice = 3;
    for(int i = 0; i < numberOfDevice; i++) {
        
        Ptr<Node> tmpNode;
        Ptr<WurCommonNetDevice> tmpDevice;
        Ptr<WurCommonPhy> tmpPhy;
        Ptr<WurCommonPhy> tmpWurPhy;
        Ptr<WurSharedMac> tmpMac;
        Ptr<MobilityModel> tmpMobility;
        
        tmpDevice = CreateObject<WurCommonNetDeviceDummyImpl>();
        tmpDevice->SetWakeUpSequenceList(wakeUpSequenceList);
        
        tmpNode = CreateObject<Node>();
        
        tmpPhy = CreateObject<WurCommonPhyDummyImpl>();
        tmpWurPhy = CreateObject<WurCommonPhyDummyImpl>();
        
        tmpMac = CreateObject<WurSharedMacDummyImpl>();
        tmpMobility = CreateObject<ConstantPositionMobilityModel>();
        
        tmpMobility->SetPosition(Vector3D(distr(gen), distr(gen), distr(gen)));
        
        tmpPhy->SetMobility(tmpMobility);
        tmpWurPhy->SetMobility(tmpMobility);
        
        tmpPhy->SetTxGain(0);
        tmpPhy->SetRxGain(0);
        tmpPhy->SetRxSensitivity(-83);
        
        tmpWurPhy->SetRxGain(0);
        tmpWurPhy->SetTxPower(20);
        tmpWurPhy->SetRxSensitivity(-83);
        tmpWurPhy->SetTxGain(0);
        
        tmpDevice->SetSharedMac(tmpMac);
        tmpDevice->SetMainRadioPhy(tmpPhy);
        tmpDevice->SetWurRadioPhy(tmpWurPhy);
        
        mainRadioChannel->Add(tmpPhy);
        wakeUpRadioChannel->Add(tmpWurPhy);
        
        tmpMac->SetNetDevice(tmpDevice);
        
        tmpPhy->SetChannel(mainRadioChannel);
        tmpPhy->SetDevice(tmpDevice);
        
        tmpWurPhy->SetChannel(wakeUpRadioChannel);
        tmpWurPhy->SetDevice(tmpDevice);
        
        tmpPhy->SetRxOkCallback(
            MakeCallback(&WurSharedMac::OnDataRx, tmpMac)
        );
        
        tmpPhy->SetTxOkCallback(
            MakeCallback(&WurSharedMac::NotifyTx, tmpMac)
        );
        
        tmpWurPhy->SetRxOkCallback(
            MakeCallback(&WurSharedMac::OnWurRx, tmpMac)
        );
        
        tmpWurPhy->SetTxOkCallback(
            MakeCallback(&WurSharedMac::OnWurTx, tmpMac)
        );
        
        
        tmpNode->AddDevice(tmpDevice);
        
        tmpMac->SetAddress(Mac8Address(tmpMac->GetNetDevice()->GetNode()->GetId()));
        socket.SetPhysicalAddress(tmpNode->GetDevice(0)->GetAddress());

        tmpPhy->TurnOff();
        tmpWurPhy->TurnOn();
    }
 
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
    apps.Stop(Seconds(1.9));
    senderPhy->TurnOff();
    senderWurPhy->TurnOn();
 
    std::cout << "Starting simulation" << std::endl;
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
    std::cout << "End of simulation" << std::endl;
}