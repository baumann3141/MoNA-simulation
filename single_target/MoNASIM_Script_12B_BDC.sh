#!/bin/sh
#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.
#Param1=$1
#Param2=$2
#printf "Param1 (E):      ${Param1}\n"
#printf "Param2 (Gamma):  ${Param2}\n"

FileName="12B_BDC"
#InputFile="input.txt"

#arguments="-geant -reac glaub_noDecay -n 100000 -glaub 1 -strag 0 -be 79.85"
#arguments="-reac glaub_noDecay -n 100000 -glaub 1 -strag 0 -be 79.85 -nn 0 -np 3"
arguments="-n 100000 -f root -be 43.94031 -bZ 5 -bA 12 -nn 0 -np 0"

#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 1 -m manual -s vison"
controlargs="-n 0"

#fragment for mona_analysis.cc
FRAGname="12B_e15091"

#MoNA Detector Config File
#MoNADetFile="detector_config_jones"
#MoNADetFile="detector_config_Calem"
#MoNADetFile="detector_config_temp"
#MoNADetFile="detector_config_Kuchera15Be"
MoNADetFile="detector_config"

#Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=5719

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

#Paths to access the executables, and to save the data files.
analysispath="/projects/mona-sim/votaw_stmona"
stmonapath="/projects/mona-sim/votaw_stmona/st_mona"
n2geantpath="/projects/mona-sim/votaw_stmona/n2_geant"
#DataPath="/evtdata/08016a/simdata/kohley_simdata/O25Calem"
#DataPath="/projects/08026/zach_hope_analysis/monasim_files/"
#DataPath="/mnt/simulations/MoNAsims/test_dump"
DataPath="/mnt/analysis/e15091/simulations"

#used to allow multiple geant runs simultaneously 
#UniqueID="_E400_W800_mil"
#UniqueID="_testing_3ComponentBeam"
UniqueID="_e15091"

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
#cd ${n2geantpath}/control
#control.py -i ${PreGeant} -o ${PostGeant} -t 0.38 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}

#Analysis
#Needed for reconstructed target parameters
#cd ${stmonapath}
#./bin/mona_analysis -frag ${FRAGname} -if ${PostGeant} -of ${FinalName} #-dT 5.0


#Dump Edecay Histograms
#needed for running large jobs and minimzing space used
#cd ${analysispath}
#root -q -b 'DumpHistsMoNASim_O26.C("'${HistDumpName}'","'${PostGeant}'","'${FinalName}'")'


#Delete created event (tree) root files
#cd ${DataPath}
#rm ${PreGeant} ${PostGeant} ${FinalName}
