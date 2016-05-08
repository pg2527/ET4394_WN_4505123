#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
//#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/stats-module.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/trace-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-helper.h"
#include "string.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std ;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PG_NS3_Assignment");

int main (int argc, char *argv[])
{
	//for (int d=20; d<=100; d=d+5)
	//{
	//	std::cout << "::\n";
	//for (int i=2;i<=16;i=i+2)
  //{
	//{
  ofstream log_file("log_values.csv", std::ios::out | std::ios::app);	
  bool udp = true;
  bool verbose=false;
  double time_simulation = 10; //seconds
  int distance = 5; //meters
  double freq_wifi = 5.0; //whether 2.4 or 5.0 GHz
  int nStanodes=i;   // number of station nodes
  int nApnodes = 1;
  double payloadSize=1472;// Payloadsize can be 1472,500,100
  double dataratevalue=3; // datarate mode
  int mobility_model = 2 ; // Type of Mobility model
  int PositionAllocator= 3;

  //Passing command line variables and parameters

  CommandLine cmd;
  cmd.AddValue ("freq_wifi", "Whether working in the 2.4 or 5.0 GHz band (other values gets rejected)", freq_wifi);
  cmd.AddValue ("distance", "Distance in meters between the station and the access point", distance);
  cmd.AddValue ("time_simulation", "Simulation time in seconds", time_simulation);
  cmd.AddValue ("udp", "UDP if set to 1, TCP otherwise", udp);
  cmd.AddValue ("nStanodes", "Number of stations", nStanodes);
  cmd.AddValue ("nApnodes","Number of Access points",nApnodes);
  cmd.AddValue ("payloadSize", "payloadsize for UDP", payloadSize);
  cmd.AddValue ("dataratevalue", "datarate value for MAC", dataratevalue);
  cmd.AddValue("mobility_model","Different Types of Mobility Model",mobility_model);
  cmd.AddValue("PositionAllocator","Different types of orientation for the nodes",PositionAllocator);
  cmd.Parse (argc,argv);

//Creating wi-fi stations and access points

  NodeContainer wifiStaNode;
  wifiStaNode.Create (nStanodes);
  NodeContainer wifiApNode;
  wifiApNode.Create (nApnodes);

  // Setting up Mac Layer and wi-fi Physical channel parameters

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  //channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  //channel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());
  phy.Set ("ShortGuardEnabled", BooleanValue (true)); 
  //phy.Set ("RxGain", DoubleValue (0));

  WifiHelper wifi = WifiHelper::Default (); 
  if(freq_wifi==5)
                {wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
                }
  else{
                wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);       
                }
  if (verbose)
    {
      wifi.EnableLogComponents ();
     }

  HtWifiMacHelper mac = HtWifiMacHelper::Default ();
  StringValue DataRate = HtWifiMacHelper::DataRateForMcs (dataratevalue);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", DataRate,
                                            "ControlMode", DataRate);        
  
  Ssid ssid = Ssid ("ns3-80211n");
  mac.SetType ("ns3::StaWifiMac",
                           "Ssid", SsidValue (ssid),
                           "ActiveProbing", BooleanValue (false));

  //Setting up devices
  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
                           "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (20));
  //Setting up mobility of stations and access points
  MobilityHelper mobility;
  switch(PositionAllocator)
  {
    case 1:   
            //cout<<"\n"<<"----Random Disc Position Allocator--------"<<endl;
        mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
        "X", DoubleValue (5.0),
        "Y", DoubleValue (5.0),
        "Theta",  StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=6.283]"),
        "Rho",StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=200.0]"));
        break;
    case 2:  
            //cout<<"\n"<<"----Grid Position Allocator-----"<<endl;
        mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
        "MinX", DoubleValue (0.0),
        "MinY", DoubleValue (0.0),
        "DeltaX", DoubleValue (15.0),
        "DeltaY", DoubleValue (15.0),
        "GridWidth", UintegerValue (4),
        "LayoutType", StringValue ("ColumnFirst"));
        break;
    case 3: 
            //cout<<"\n"<<"----Uniform Disc Position Allocator-----"<<endl;    
          mobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
          "X", DoubleValue (5.0),
          "Y", DoubleValue (5.0),
          "rho", DoubleValue (5.0));
          break;
    case 4:
          //cout<<"\n"<<"-----Random Rectangle Position Allocation----"<<endl
          mobility.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
          "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"),
          "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"));
          break;
    case 5:   
          //cout<<"\n"<<"---------Random Box Position Allocator----"<<endl
          mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
          "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"),
          "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"),
          "Z", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"));
          break;
    default:
          std::cout<<"(0,0) positions assigned to all nodes";

  }              
             
             
switch(mobility_model)
{
    case 1:
       //std::cout << "RandomDirection2d" << "\t";
       mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel", "Bounds",
       RectangleValue (Rectangle (-10, 10, -10, 10)), "Speed",
       StringValue ("ns3::ConstantRandomVariable[Constant=3]"), "Pause",
       StringValue ("ns3::ConstantRandomVariable[Constant=0.4]"));
       break;
    case 2:
        //std::cout << "RandomWalk2d" << "\t";
        mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
        "Bounds", RectangleValue (Rectangle (-1000, 1000, -1000, 1000)),
        "Distance", ns3::DoubleValue (300.0));
        break;
    case 3:
         //std::cout << "ConstantPosition" << "\t";
         mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
         break;
    default:
          std::cout << "RandomWalk2d" << "\t";
          break;
}

mobility.Install (wifiStaNode);
//NS_LOG_INFO("Station Points mobility is configured");
              
//Varying the distance of Access Point from the Stations

MobilityHelper mobility1;			
Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
positionAlloc->Add (Vector (distance, 0.0, 0.0));
mobility1.SetPositionAllocator (positionAlloc); 
mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility1.Install (wifiApNode);
//NS_LOG_INFO("Access Point mobility is configured");             
			
  
/*Installing Internet stack*/
InternetStackHelper stack;
stack.Install (wifiApNode);
stack.Install (wifiStaNode);

Ipv4AddressHelper address;

address.SetBase ("192.168.1.0", "255.255.255.0");
Ipv4InterfaceContainer staNodeInterface;
Ipv4InterfaceContainer apNodeInterface;

staNodeInterface = address.Assign (staDevice);
apNodeInterface = address.Assign (apDevice);

//Initialising Server and Client apps on station and access point nodes

ApplicationContainer serverApp;
UdpServerHelper myServer (9);
serverApp = myServer.Install (wifiStaNode.Get (0));
serverApp.Start (Seconds (1.0));
serverApp.Stop (Seconds (time_simulation));

UdpClientHelper myClient (staNodeInterface.GetAddress (0), 9);
myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295));
myClient.SetAttribute ("Interval", TimeValue (Seconds (1))); 
myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));

ApplicationContainer clientApp = myClient.Install (wifiStaNode); 
clientApp.Start (Seconds (2.0));
clientApp.Stop (Seconds (time_simulation));
                            
Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
//Calculating Average Throughput and Delay using FlowMonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.Install(wifiStaNode);
//Checking for lost packets          
monitor->CheckForLostPackets ();
Simulator::Stop (Seconds (time_simulation + 1));
//NS_LOG_INFO("Starting the Simulation"); 
Simulator::Run ();
double avg_throughput = 0;
double throughput=0;
double avg_delay=0;
bool flag =false;
Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
{
throughput=i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/nStanodes;
//std::cout << " Average Throughput: " << throughput << " kbps\n";
if (throughput==0)
	{
        flag=true;
	}
	else
	{
		
		avg_delay=avg_delay+ (i->second.delaySum / i->second.rxPackets).ToDouble(Time::US);
	  //cout<<avg_delay<<"\n";
		} 
     
     avg_throughput=avg_throughput+throughput;
}
            
NS_LOG_INFO("Stopping the Simulation");              
Simulator::Destroy ();
//Calculating Average Throughput and Average Delay                   
avg_delay=avg_delay/(nStanodes);
avg_throughput=avg_throughput/nStanodes;
log_file<<nStanodes<<","<<avg_delay<<","<<avg_throughput<<"\n";
//log_file << "Number_of_nodes,Average_throughput_udp,Average_delay,Mobility_model,data_rate\n";
//log_file << nStanodes << "," << avg_throughput << "," << avg_delay << "," << mobility_model<< "," <<dataratevalue<<"\n";
std::cout <<": with" << dataratevalue << "\t" << distance  << "\t" <<  payloadSize << "\t" << nStanodes << "\t"<< avg_throughput << " \t" <<"delay is"<< avg_delay << "\t" <<flag<< std::endl;

//}
return 0;
}
