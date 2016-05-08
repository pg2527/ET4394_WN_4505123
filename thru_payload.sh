#OUTPUT_STATS=out_stats.dat
#PLOT_THROUGHPUT=out_throughput.dat
THROUGHPUT_IMG=throughput_img_rate.png

rm out_stats_*
rm plot_tp_pl*

#for rate in 1 2 3 4  
for payload in 1472 500 100  
do
	for node in 1 2 3 4 5  
	do
		echo -ne "Simulating for $node nodes with rate $rate ... "
		#./waf --run "scratch/ns3_finalassign --nStanodes=$node --dataratevalue=$rate" >> out_stats_rate_$rate.dat
		./waf --run "scratch/ns3_finalassign --nStanodes=$node --payloadSize=$payload" >> out_stats_pl_$payload.dat
		#./waf --run "scratch/final --nStanodes=$node " >> out_stats_.dat
		echo "DONE"
	done

	sed -i "/\b\(waf\|build\|Compiling\)\b/d" out_stats_pl_$payload.dat
	#sed -i "/\b\(waf\|build\|Compiling\)\b/d" out_stats_.dat
	cut -f1,3 out_stats_pl_$payload.dat > plot_tp_pl_$payload.dat
	#cut -f1,2 out_stats_.dat > plot_throughput_.dat
done

gnuplot <<- EOF
#set title "Network Performance vs Data Rate"
set title "Network Performance vs Payload"
set xrange [0:]
set xlabel "Number of Nodes"
set ylabel "Average Throughput (Mbps)"
set term png
set output "$THROUGHPUT_IMG"
set style data linespoints
#plot 'plot_tp_rate_1.dat' w l lc rgb 'yellow' title "1",'plot_tp_rate_2.dat' w l lc rgb 'green' title "2",'plot_tp_rate_3.dat' w l lc rgb 'blue' title "3"
plot 'plot_tp_pl_1472.dat' w l lc rgb 'yellow' title "1",'plot_tp_pl_500.dat' w l lc rgb 'green' title "2",'plot_tp_pl_100.dat' w l lc rgb 'blue' title "3"
#'plot_tp_rate_4.dat' w l lc rgb 'red' title "4"
EOF
xdg-open $THROUGHPUT_IMG
