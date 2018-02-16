#!/bin/bash

#Handle input arguments
#First argument should be total energy.
#Second argument is total width.
#Third argument is intermediate energy.
#Fourth argument is intermediate width.
e1=$1
w1=$2
e2=$3
w2=$4

#From beam fitting
beam_flags="-n 500000 -be 71 -dbe 0.004 -scale1 2 -scale2 15 -scale3 1
-cx -0.002 -ctx -0.005 -dx 0.008 -dtx 0.005 -sxtx -0.82
-cy 0.005 -cty 0.0055 -dy 0.002 -dty 0.0005 -syty 0
-cx2 -0.002 -ctx2 -0.005 -dx2 0.0075 -dtx2 0.002 -sxtx2 -0.82
-cy2 0.00 -cty2 0.000 -dy2 0.007 -dty2 0.009 -fy2 10 -fty2 20 -syty2 -0
-cx3 0.00 -ctx3 0.001 -dx3 0.004 -dtx3 0.002 -sxtx3 -0.82
-cy3 -0.012 -cty3 0.000 -dy3 0.0005 -dty3 0.0009 -syty3 -0 -sxy3 0.5"

analysis=""

temp_dir="/mnt/simulations/MoNA_11Li"
geant_flags="-d ${temp_dir} -t 0.6"

sim_dir="/projects/mona-sim/smithj/st_mona/root_files/11li_decay/path1"

#factors for conversion from width to SF input
prefactor91=1.197526476
denomfactor91=0.806664184
prefactor101=1.232657185
denomfactor101=0.854687101

eprint1=`echo "hello" | awk -v e=$e1 '{print e*1000}'` #Format energy for printing to filename
wprint1=`echo "hello" | awk -v e=$w1 '{print e*1000}'` #Format width for printing to filename
eprint2=`echo "hello" | awk -v e=$e2 '{print e*1000}'` #Format energy for printing to filename
wprint2=`echo "hello" | awk -v e=$w2 '{print e*1000}'` #Format width for printing to filename
spw2=`echo "hello" | awk -v p=$prefactor91 -v d=$denomfactor91 -v e=$e2 '{print p*e^(3/2)/(1+d*e)}'`
sf2=`echo "hello" | awk -v w=$w2 -v spw=$spw2 '{print w/spw}'`
spw1=`totalwidth 1 $e1 $e2 1 $sf2 9`
sf1=`echo "hello" | awk -v w=$w1 -v spw=$spw1 '{print w/spw}'`

flags="-geant -reac 2neutron -e Volya_2nSeq $e1 $e2 $sf1 $sf2 1 0.001 -glaub 1.5 -strag 1 -glaubtrans 2 0 -asymMomentum 2" #set flags for simulation

filename="$temp_dir/9li_2n_1e${eprint1}_1w${wprint1}_2e${eprint2}_2w${wprint2}_l1"
simfile="$sim_dir/9li_2n_1e${eprint1}_1w${wprint1}_2e${eprint2}_2w${wprint2}_l1.root" #declare histogram filename
uid="path1_1e${eprint1}_1w${wprint1}_2e${eprint2}_2w${wprint2}" #declare unique name for multiple geant sessions.

#test to see if histogram file already exists. if so, continue to next iteration - don't waste time running simulation
if [ -e $simfile ]; then
	exit
fi

#run simulation
./st_mona -exp e10007-prep -f ${filename}.root $beam_flags $flags
cd ../../n2_geant/control
./control.py -i ${filename}.root -o ${filename}_ag.root -n 2 -u ${uid} $geant_flags
cd ../../st_mona/bin
./mona_analysis -if ${filename}_ag.root -of ${filename}_ana.root -frag 9Li $analysis

#Pull desired histograms from simulation files
root.exe -b <<EOF
TFile* _file0 = TFile::Open("${filename}_ag.root");
gROOT->ProcessLine(".L ./fragment_spectra.C");
gROOT->ProcessLine(".x notch.C");
gROOT->ProcessLine(".x sweep_sim.C");
t->SetAlias("icfrontx","b9p0x*1000+114.3*tan(b9p0tx)");
t->SetAlias("icbackx","b9p0x*1000+825*tan(b9p0tx)");
t->SetAlias("icbacky","b9p0y*1000+825*tan(b9p0ty)");
draw_sim_hsts_9Li2n("$simfile","${filename}_ana.root","abs(icbackx)<200 && abs(icbacky)<200 && b9p0y*1000<85 && b9p0y*1000>-150 && abs(b7p0y*1000)<20 &! notch && vG1NPos.fX!=0 && b9p0Ekin!=0 && sweep && abs(icfrontx)<150 && b9p0x>-0.12");
gROOT->ProcessLine(".q");
EOF

rm -f ${filename}.root
rm -f ${filename}_ag.root
rm -f ${filename}_ana.root
rm -f ${temp_dir}/*${uid}*
