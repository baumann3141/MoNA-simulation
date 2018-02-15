#!/bin/bash

#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.
TargetID=$1
#LIFETIME=$2
printf "Reaction Target:      ${TargetID}\n"
#printf "Lifetime:      ${LIFETIME}\n"

# Clean up
#OUTFILE="/projects/mona-sim/redpath/output.out"
#LIFEFILE="/projects/mona-sim/redpath/lifetimes.out"
#RXNFILE="/projects/mona-sim/redpath/rxnpt.out"
#EXNFILE="/projects/mona-sim/redpath/exenergies.out"
#DEFILE="/projects/mona-sim/redpath/deout.out"
#ANGFILE="/projects/mona-sim/redpath/angles1n.out"
#if [ -f $OUTFILE ];
#then
#  echo "Removing old output file ..."
#  rm -f ${OUTFILE}
#fi
#if [ -f $LIFEFILE ];
#then
#  echo "Removing old lifetimes file ..."
#  rm -f ${LIFEFILE}
#fi

#if [ -f $RNXFILE ];
#then
#  echo "Removing old rxnpts file ..."
#  rm -f ${RXNFILE}
#fi

#if [ -f $EXNFILE ];
#then
#  echo "Removing old exenergies file ..."
#  rm -f ${EXNFILE}
#fi

#if [ -f $DEFILE ];
#then
#  echo "Removing old deout file ..."
#  rm -f ${DEFILE}
#fi

#if [ -f ${ANGFILE} ];
#then
#  echo "Removing old angles file ..."
#  rm -f ${ANGFILE}
#fi

### Set the name for the output root files here ###
FileName="ST_F27O24"

### Here is where you set the incident beam parameters,
### the glauber kick, the reaction type and parameters.
### All of these flags are explained in more detail in
### the opening comments of st_mona.cc. I've left a few
### examples to demonstrate how i've simulated different
### decays:

#### ---- e15118 O26 -> O24 + 2n ---- ####

#### O-24 from F-27 ####
arguments=" -exp 15118_O26 -geant -glaub 0.5 -be 105.652 -dbe 3.0 -reac 3body_decay -lifetime 0 -e const 0.05 -n 1000 -ctx 0.003 -cty -0.002 -cx 0.012 -cy -0.016 -dtx 0.00001 -dty 0.00001 -dx 0.00001 -dy 0.00001 -slice ${TargetID}" #-n number of events.


### The geant simulation is controlled through a python script control.py
### (-n number of neutrons)
#controlargs="-n 1 -m manual -s vison"
#controlargs="-n 2"
controlargs="-n 2 -g ${TargetID}"
#controlargs="-n 2 -g 1"

#fragment for mona_analysis.cc
FRAGname="O24"

### Specify the MoNA Detector Config File which
### should sit in the n2_geant/ directory

MoNADetFile="detector_config_surveyOct2017"
#MoNADetFile="detector_config_R602_final"
#MoNADetFile="detector_config_HopeO26"

#Change the random seed if long runs are needed, but you want to do them as several short runs.
#export GSL_RNG_SEED=55395
export GSL_RNG_SEED=55496

### for variety of pseudo data ###
### By taking the random seed from the
### current time, the RNGs are initialized
### differently, in principle, everytime
#now=$(date +%N)
#export GSL_RNG_SEED=${now:2:5}

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

### Paths to access the executables
stmonapath="/projects/mona-sim/baumann/st_mona"
stmonapathred="/projects/mona-sim/baumann/st_mona"
inputpath="/projects/mona-sim/baumann/st_mona/input_files"
n2geantpath="/projects/mona-sim/baumann/n2_geant"

### Path to save data files
analysispath="/mnt/simulations/MoNAsims/baumann"
DataPath="/mnt/simulations/MoNAsims/baumann"

#used to allow multiple geant runs simultaneously 
UniqueID="_CycSrimANDdEfix_NoRxn_${TargetID}"

#Make file names based on the file name given.
PreGeant=${DataPath}/${FileName}"${UniqueID}.root"
PostGeant=${DataPath}/${FileName}"-geant${UniqueID}.root"
FinalName=${DataPath}/${FileName}"-geant-anal${UniqueID}.root"
HistDumpName=${DataPath}/${FileName}"-hist${UniqueID}.root"
DefaultName=${DataPath}/"default${UniqueID}.root"
MacTempName=${DataPath}/"temp${UniqueID}.mac"
InDatTempName=${DataPath}/"temp${UniqueID}.indat"
NeutronOne=${DataPath}/"neutron1${UniqueID}.root"
TempNeutronOne=${DataPath}/"neutron1temp${UniqueID}.indat"
NeutronTwo=${DataPath}/"neutron2${UniqueID}.root"
TempNeutronTwo=${DataPath}/"neutron2temp${UniqueID}.indat"

#Run the simulations.
#st_mona simulation
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}
echo '-------Finished st_mona-------'

#geant afterburner
#Using light threshold of 0.15 MeVee.
cd ${n2geantpath}/control
control.py -i ${PreGeant} -o ${PostGeant} -t 0.38 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}


#Dump Edecay Histograms
#needed for running large jobs and minimzing space used
#cd ${analysispath}
#root -q -b 'DumpHistsMoNASim_O26_2nCorr.C("'${HistDumpName}'","'${PostGeant}'","'${FinalName}'")'

#printf "Param1 (Lifetime):      ${G}\n"

#Delete created event (tree) root files
cd ${DataPath}
#rm ${PreGeant} ${PostGeant} ${FinalName}
