# ET4394_WN_4505123
Wireless Networking Assignments: Parul Gupta (p.gupta-3@student.tudelft.nl)

## GNU Project
* The Project report is present with the title "wireless-networking-gnu-4505123.pdf:
* Probability curves and ROC curves are plotted using Matlab and script is provided with the title "plot_gnusdr.m".
* Use "run plot_gnusdr.m" in the command window to run the script.

## NS3 Project
* The ns3_finalassign.cc file contains the network topology which was created and simulated to generate results.
* Copy the file to the scratch folder of the NS-3.
* To run the file type " ./waf --run scratch/ns3_finalassign".
* Command line parameters can be passed as per the below mentioned instruction -
  * --udp : To set up udp traffic;
  * --verbose=false/true : to set up verbose output
  *--time_simulation = 10 or any other integer //to add integer value of time
  * --distance = 5 or any other integer; //for distance between station and access points
  * --double freq_wifi = 5.0 or 2.4; //whether 2.4 or 5.0 GHz
  * --nStanodes=5 or any other integer;   // number of station nodes
  * --nApnodes = 1; // No of access points
  * --payloadSize=1472;// Payloadsize can be 1472,500,100
  * --dataratevalue=3; // datarate mode
  * --mobility_model = 2 ; // Type of Mobility model
  * --PositionAllocator= 3; // type of orientation of nodes
* Run thru_payload.sh and delay_payload.sh to see the graphs related to effect of payload on throughput and delay.This can be taken as a sample of generated graphs.
* Use generated CSVs for analysis
* Also use "out_stats_*" and "plot_tp_*" files generated to get the glimpse of how analysis was done.

# References :
 *Works by (hedi02)[https://github.com/hedi02] and Rizqi
