#!/bin/sh

#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.

P1=$1
P2=$2
P3=$3
P4=$4
#P5=$5
printf "Param1 (Ei): ${P1}\n"
printf "Param2 (N):  ${P2}\n"
printf "Param2 (Ev): ${P2}\n"
printf "Param3 (SV): ${P3}\n"
#printf "Param4 (L): ${P4}\n"
#printf "Param5 (GammaIn): ${P5}\n"


#FileName="Be14_He8+3n"
FileName="Be14_He8+4n"

#thermal bckg
#arguments="-exp 09067_He10 -geant -reac 3body_decay -e therm ${P1}  -n 100000 -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.
#thermal bckg MDJ
arguments="-exp 09067_He10 -geant -reac 5body_decay -e asymbw ${P1} ${P2} 1 0 -n ${P3}  -nn 0 -np 2 -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.


#THERE ARE RULES TO THIS IMPLEMENTATION OF A SEQUENTIAL DECAY
#-e sets the TOTAL ENERGY AVAILABLE with respect to Helium 8 (therefore you must subtract the energy of 10He)
#-e1 set the energy of the first neutron from 12He (gauss at -e/2)
#-e2 will be set by energy conservation ( e - e1)
#-e3 should be the 10He resonance (1.6 bw)
#-e4 does not matter, set to const 0.01

#arguments="-exp 09067_He10 -geant -reac 5body_seq -e1 const ${P1} -e2 gauss ${P2} ${P3} -e3 asymbw 1.6 1.8 1 0  -e4 const 0.01 -n ${P4} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.




#4n efficiency
#arguments="-exp 09067_He10 -reac 5body_decay -e uniform 0.01 5 -e1 const 0.01 -e2 const 0.01 -e3 const 0.01 -e4 const 0.01 -n ${P1} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#3n efficiency
#arguments="-exp 09067_He10 -geant -reac 4body_decay -e uniform 0.05 10 -e1 const 0.01 -e2 const 0.01 -e3 const 0.01 -n 100000 -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#arguments="-exp 09067_He10 -geant -reac 3neutron -e1 therm ${P1} -e2 therm ${P1} -e3 therm ${P1} -n ${P2} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#arguments="-exp 09067_He10 -geant -reac 5body_decay -e asymbw ${P1} ${P2} ${P3} 0 -n ${P4} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#arguments="-exp 09067_He10 -geant -reac 5body_decay -e uniform 0.05 10 -n ${P1} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.




#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 2 -m manual -s vison"
controlargs="-n 4"

#fragment for mona_analysis.cc
FRAGname="He8_09067"

#MoNA Detector Config File
MoNADetFile="detector_config_jesse09067"

#Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=527391

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

#Paths to access the executables, and to save the data files.
#analysispath="/projects/mona-sim/jonesm"
analysispath="/user/jonesm/tetra_neutrons/He10"
stmonapath="/projects/mona-sim/jonesm/st_mona"
n2geantpath="/projects/mona-sim/jonesm/n2_geant"
#DataPath="/mnt/simulations/MoNA_EfficiencyTest/Be14_He10/fine_mesh/4N_Simulations/5body/"

#used to allow multiple geant runs simultaneously 
#UniqueID="_Volya_Ei${P1}_Ev${P2}_SV${P3}_L${P4}_Gin${P5}"
#UniqueID="_4n_5body_E${P1}_G${P2}_L${P3}"
#UniqueID="_T${P1}MeV"
#UniqueID="_test"
UniqueID="_5body_E${P1}_G${P2}_L1"
#UniqueID="_eff"
#UniqueID="_He10_GS_test_newSweeper_RedVel"
#UniqueID="_1stEx_Ref48_thru_1.27"
#UniqueID="_Ref48_1stEx_Seq_thru_1.27"
#UniqueID="_Ref48_2ndEx_Seq_thru_2.42"
#UniqueID="_CorrBckg_din"
#UniqueID="_broadExcited_L0"
#UniqueID="_broadExcited_4.31_uniform"
#Make file names based on the file name given.
PreGeant=${DataPath}/${FileName}"${UniqueID}.root"
PostGeant=${DataPath}/${FileName}"-geant${UniqueID}.root"
FinalName=${DataPath}/${FileName}"-geant-anal${UniqueID}.root"
HistDumpName=${DataPath}/${FileName}"-hist${UniqueID}.root"

#Run the simulations.

#st_mona simulation
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}

#geant afterburner
#Using light threshold of 0.15 MeVee.
cd ${n2geantpath}/control
control.py -i ${PreGeant} -o ${PostGeant} -t 0.38 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}

#Analysis
#Needed for reconstructed target parameters
cd ${stmonapath}
./bin/mona_analysis -frag ${FRAGname} -if ${PostGeant} -of ${FinalName}

#Dump Edecay Histograms
#needed for running large jobs and minimzing space used
cd ${analysispath}
root -q -b 'DumpHistsMoNASim_12He.C("'${HistDumpName}'","'${PostGeant}'","'${FinalName}'")'

#Delete created event (tree) root files
cd ${DataPath}
rm neutron*
rm ${PreGeant} ${PostGeant} ${FinalName}
