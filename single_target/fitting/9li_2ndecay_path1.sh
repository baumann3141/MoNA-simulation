#!/bin/bash

#From beam fitting
beam_flags="-n 5000000 -be 71 -dbe 0.004 -ratio 0.22
-cx -0.001 -ctx -0.0038 -dx 0.008 -dtx 0.005 -sxtx -0.82
-cy 0.005 -cty 0.0055 -dy 0.002 -dty 0.0005 -syty 0
-cx2 -0.001 -ctx2 -0.0038 -dx2 0.0075 -dtx2 0.002 -sxtx2 -0.82
-cy2 0.002 -cty2 0.000 -dy2 0.007 -dty2 0.007 -fy2 10 -fty2 6 -syty2 -0"

analysis=""

sim_dir="/projects/mona-sim/smithj/st_mona/root_files/11li_decay/path1"
temp_dir="/mnt/simulations/MoNA_11Li"
geant_flags="-d ${temp_dir} -t 0.6"
#geant_flags="-d /projects/mona-sim/smithj/n2_geant/control -t 0.35"

#Set energies and widths here
#energies2="0.55"
#widths2="0.8"
energies2="0.50 0.525 0.55 0.575 0.6"
widths2="0.5 0.55 0.6"
#For the 700 keV (ish) decay
#energies1="0.8"
#widths1="1.0"
energies1="0.65 0.7 0.75 0.8"
widths1="0.7 0.8 0.9 1.0"

#factors for conversion from width to SF input
prefactor91=1.197526476
denomfactor91=0.806664184
prefactor101=1.232657185
denomfactor101=0.854687101

#Loop over possible energies
for e1 in $energies1
do
    eprint1=`echo "hello" | awk -v e=$e1 '{print e*1000}'` #Format energy for printing to filename
    
	#Loop over widths
    for w1 in $widths1
    do
	wprint1=`echo "hello" | awk -v e=$w1 '{print e*1000}'` #Format width for printing to filename
	
		#Loop over second energies
	for e2 in $energies2
	do
	    eprint2=`echo "hello" | awk -v e=$e2 '{print e*1000}'` #Format energy for printing to filename
	    
			#Loop over second widths
	    for w2 in $widths2
	    do
		wprint2=`echo "hello" | awk -v e=$w2 '{print e*1000}'` #Format width for printing to filename
		spw2=`echo "hello" | awk -v p=$prefactor91 -v d=$denomfactor91 -v e=$e2 '{print p*e^(3/2)/(1+d*e)}'`
		echo $spw2
		sf2=`echo "hello" | awk -v w=$w2 -v spw=$spw2 '{print w/spw}'`
		echo $sf2
		spw1=`totalwidth 1 $e1 $e2 1 $sf2 9`
		echo $spw1
		sf1=`echo "hello" | awk -v w=$w1 -v spw=$spw1 '{print w/spw}'`
		echo $sf1
		#set flags for simulation
		flags="-geant -reac 2neutron -e Volya_2nSeq $e1 $e2 $sf1 $sf2 1 0.001 -legendre_lvalue 1 -glaub 1 -strag 1 -glaubtrans 2 0" 
		simfile="$sim_dir/9li_2n_1e${eprint1}_1w${wprint1}_2e${eprint2}_2w${wprint2}_l1.root" #declare histogram filename
		uid="path1_1e${eprint1}_1w${wprint1}_2e${eprint2}_2w${wprint2}" #declare unique name for multiple geant sessions.
		
				#test to see if histogram file already exists. if so, continue to next iteration - don't waste time running simulation
		if [ -e $simfile ]; then
		    continue
		fi
		
				#run simulation
		./st_mona -exp e10007-prep -f 9li_frag.root $beam_flags $flags
		cd ../../n2_geant/control
		./control.py -i ../../st_mona/bin/9li_frag.root -o ../../st_mona/bin/9li_frag_ag.root -n 2 -u $uid $geant_flags
		cd ../../st_mona/bin
		./mona_analysis -if 9li_frag_ag.root -of 9li_frag_ana.root -frag 9Li $analysis
		
#Pull desired histograms from simulation files
		root.exe -b <<EOF
TFile* _file0 = TFile::Open("9li_frag_ag.root");
gROOT->ProcessLine(".L ./fragment_spectra.C");
gROOT->ProcessLine(".x notch.C");
gROOT->ProcessLine(".x sweep_sim.C");
t->SetAlias("icfrontx","b9p0x*1000+114.3*tan(b9p0tx)");
t->SetAlias("icbackx","b9p0x*1000+825*tan(b9p0tx)");
t->SetAlias("icbacky","b9p0y*1000+825*tan(b9p0ty)");
draw_sim_hsts_9Li2n("$simfile","9li_frag_ana.root","abs(icbackx)<200 && abs(icbacky)<200 && b9p0y*1000<85 && b9p0y*1000>-150 && abs(b7p0y*1000)<20 &! notch && vG1NPos.fX!=0 && b9p0Ekin!=0 && sweep && abs(icfrontx)<150 && b9p0x>-0.12");
gROOT->ProcessLine(".q");
EOF
	    done
	done
    done
done
