#!/bin/sh
#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.
#Param1=$1
#Param2=$2
#printf "Param1 (E):      ${Param1}\n"
#printf "Param2 (Gamma):  ${Param2}\n"

FileName="17C-2p1n"
#InputFile="input.txt"
#arguments="-exp 12004_dp -geant -e uniform 0.01 10 -n 10000 -strag 1 -be 82.5"
#arguments="-exp 12004_dp -geant -e asymbw 0.77 0.0000001 2 0 -n 100000 -strag 1 -be 82.4"

#Decay of state at 280 keV
#arguments="-geant -reac 3body_decay -e asymbw 0.280 0.025 2 0 -n 1000000 -strag 1 -be 54.90 -nn 1 -np 2"

#Dineutron Decay of 280 keV
#arguments="-geant -reac di_neutron -e Volya_Dineutron 0.280 0.025 -18.7 -n 1000000 -strag 1 -be 54.90 -nn 1 -np 2"

#Decay of state at 2280 keV
#arguments="-geant -reac 3body_decay -e asymbw 2.280 1.5 2 0 -n 1000000 -strag 1 -be 54.90 -nn 1 -np 2"

#Decay of state at 3990 keV
#arguments="-geant -reac 3body_decay -e asymbw 3.990 1.0 2 0 -n 1000000 -strag 1 -be 54.90 -nn 1 -np 2"

#Sequential Decay
arguments="-geant -reac 2neutron -e asymbw 4.00 1.0 2 0 -e2 asymbw 2.35 1.500 2 0 -n 1000000 -strag 1 -be 54.90 -nn 1 -np 2"
#arguments="-geant -reac 2neutron -e TwoNeutron_seq 2.280 1.5 0.40 0.8 0.0 0.5 2 -n 100000 -strag 1 -be 54.90 -nn 1 -np 2"
#arguments="-geant -reac 2neutron -e Volya_2nSeq 2.280 0.40 0.5 0.5 2 1.5 -n 100000 -strag 1 -be 54.90 -nn 1 -np 2"


#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 1 -m manual -s vison"
controlargs="-n 2"

#fragment for mona_analysis.cc
FRAGname="Be12_09067"

#MoNA Detector Config File
#MoNADetFile="detector_config_jones"
#MoNADetFile="detector_config_Calem"
#MoNADetFile="detector_config_temp"
MoNADetFile="detector_config_Kuchera15Be"

#Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=5719

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

#Paths to access the executables, and to save the data files.
analysispath="/projects/mona-sim/kuchera"
stmonapath="/projects/mona-sim/kuchera/st_mona"
n2geantpath="/projects/mona-sim/kuchera/n2_geant"
#DataPath="/evtdata/08016a/simdata/kohley_simdata/O25Calem"
#DataPath="/projects/08026/zach_hope_analysis/monasim_files/"
DataPath="/mnt/simulations/MoNAsims/test_dump"

#used to allow multiple geant runs simultaneously 
UniqueID="_E4000_W1000_mil_092514"

#Make file names based on the file name given.
PreGeant=${DataPath}/${FileName}"${UniqueID}.root"
PostGeant=${DataPath}/${FileName}"-geant${UniqueID}.root"
FinalName=${DataPath}/${FileName}"-geant-anal${UniqueID}.root"
HistDumpName=${DataPath}/${FileName}"-hist${UniqueID}.root"

#Run the simulations.

#st_mona simulation
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}
#echo '-------Finished st_mona-------'

#geant afterburner
#Using light threshold of 0.15 MeVee.
cd ${n2geantpath}/control
control.py -i ${PreGeant} -o ${PostGeant} -t 0.38 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}

#Analysis
#Needed for reconstructed target parameters
cd ${stmonapath}
./bin/mona_analysis -frag ${FRAGname} -if ${PostGeant} -of ${FinalName} #-dT 5.0


#Dump Edecay Histograms
#needed for running large jobs and minimzing space used
#cd ${analysispath}
#root -q -b 'DumpHistsMoNASim_O26.C("'${HistDumpName}'","'${PostGeant}'","'${FinalName}'")'


#Delete created event (tree) root files
#cd ${DataPath}
#rm ${PreGeant} ${PostGeant} ${FinalName}
