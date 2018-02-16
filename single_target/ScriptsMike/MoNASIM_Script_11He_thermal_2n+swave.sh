#!/bin/sh

#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.

#FOUR NEUTRON SIMULATIONS NEED TO HAVE 4n IN THERE NAME IF YOU WANT DUMPHISTS TO WORK PROPERLY!!

P1=$1
P2=$2
P3=$3
P4=$4
P5=$5
P6=$6
P7=$7
printf "Param1 (Ei): ${P1}\n"
printf "Param2 (Ev): ${P2}\n"
printf "Param3 (SV): ${P3}\n"
printf "Param4 (L): ${P4}\n"
printf "Param5 (GammaIn): ${P5}\n"

FileName="Be14_He8+3n"

arguments="-exp 09067_He10 -geant -reac 3neutron -e1 therm ${P1} -e2 therm ${P1} -e3 swave ${P2} ${P3} -nn 1 -np 2 -n  ${P4} -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 2 -m manual -s vison"
controlargs="-n 3"

#fragment for mona_analysis.cc
FRAGname="He8_09067"

#MoNA Detector Config File
MoNADetFile="detector_config_jesse09067"

#Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=1527391

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

#Paths to access the executables, and to save the data files.
analysispath="/user/jonesm/tetra_neutrons/He10"
stmonapath="/projects/mona-sim/jonesm/st_mona"
n2geantpath="/projects/mona-sim/jonesm/n2_geant"
DataPath="/mnt/simulations/MoNA_EfficiencyTest/Be14_He10/fine_mesh/3N_Simulations/swave+2n/thermal"
#DataPath="/mnt/simulations/MoNA_10He"

#used to allow multiple geant runs simultaneously 
#UniqueID="_Volya_Ei${P1}_Ev${P2}_SI${P3}_SV${P4}_L${P5}"
#UniqueID="_2n_3body_E${P1}_G${P2}_L${P3}_moreStats2"
#UniqueID="_2n_sequential_Etot${P1}_Gtot${P2}_E1_${P3}_G1_${P4}"
#UniqueID="_4n+dineutron_T1${P1}_T2${P2}_E${P3}_G${P4}"
#UniqueID="_4n+dineutron_T${P1}_E${P3}_G${P4}"
#UniqueID="_volya_dineutron_E${P1}_G${P2}_A${P3}"
#UniqueID="_He10_GS_test_newSweeper_RedVel"
#UniqueID="_GSI_1stEx"
#UniqueID="_Ref48_2ndEx_Seq"
#UniqueID="_3n_dineutron_test"
#UniqueID="_CorrBckg"
#UniqueID="_1stEx_GSI_thru_1.27"
#UniqueID="_3n_dineutron_T${P1}_E${P2}_G${P3}"
UniqueID="_T${P1}MeV_swave_Eb${P2}_A${P3}"
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
rm default*
rm temp*
rm ${PreGeant} ${PostGeant} ${FinalName}


