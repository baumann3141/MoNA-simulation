#!/bin/sh
#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.
#Param1=$1
#Param2=$2
#printf "Param1 (E):      ${Param1}\n"
#printf "Param2 (Gamma):  ${Param2}\n"

FileName="27F-1p3n"
#InputFile="input.txt"
#arguments="-exp 12004_dp -geant -e uniform 0.01 10 -n 10000 -strag 1 -be 82.5"
#arguments="-exp 12004_dp -geant -e asymbw 0.77 0.0000001 2 0 -n 100000 -strag 1 -be 82.4"

#phase space
arguments="-geant -reac glaub -e asymbw 0.045 0.0001 2 0 -n 400 -strag 1 -be 82.5 -nn 3 -np 1" #"ground" state #-n number of events

#dineutron
#arguments="-exp 12004_dp -geant -reac di_neutron -e Volya_Dineutron 0.15 0.005 -18.7 -n 100000 -strag 1 -be 80.0 -nn 2 -np 2" #"ground" state #-n number of events

#arguments="-exp 12004_dp  -geant -e asymbw 1.3 3.0 1 0 -n 100000 -strag 1 -be 82.4"
#arguments="-exp 12004_dp  -geant -e asymbw 1.3 0.815 1 0 -n 100000 -strag 1 -be 82.4" #first excited state
#arguments="-exp 12004_dp -geant -e asymbw 0.3 0.186 1 0 -n 100000 -strag 1 -be 82.4" #low bound on excited state
#arguments="-exp 12004_dp -geant -e asymbw 2.3 1.27 1 0 -n 100000 -strag 1 -be 82.4" #high bound on excited state
#arguments="-exp 12004_dp  -geant -e asymbw 1.3 0.00000001 1 0 -n 100000 -strag 0 -be 82.4"
#arguments="-exp 12004_dp -geant -e bw 1.3 0.2 -n 50000 -strag 1 -be 82.4" #-n number of events


#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 1 -m manual -s vison"
controlargs="-n 1"

#fragment for mona_analysis.cc
FRAGname="22O"

#MoNA Detector Config File
#MoNADetFile="detector_config_jones"
#MoNADetFile="detector_config_Calem"
#MoNADetFile="detector_config_temp"
MoNADetFile="detector_config_HopeO26"


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
UniqueID="_E045_400Events"

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
