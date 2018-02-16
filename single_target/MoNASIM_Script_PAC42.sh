#!/bin/sh
FileName="PAC42"

#arguments="-n 100000 -f root -geant -reac glaub -e swave 0.50164 9.0 -be 43.2733 -bZ 4 -bA 11 -glaub 4 -strag 1 -nn 0 -np 2"
arguments="-n 1000000 -f root -geant -reac 2body -32.5 0.3 -e swave 0.50164 9.0 -be 43.2733 -bZ 4 -bA 11 -strag 1 -nn 0 -np 2"

# For GEANT simulation through Python script control.py (-n number of neutrons).
controlargs="-n 1"

# Fragment for mona_analysis.cc.
FRAGname="He8_11Be_e15091"

# MoNA detector configuration file.
MoNADetFile="detector_config"

# Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=5719

# Paths to access the executables, and to save the data files.
analysispath="/projects/mona-sim/baumann/single_target"
stmonapath="/projects/mona-sim/baumann/single_target/st_mona"
n2geantpath="/projects/mona-sim/baumann/single_target/n2_geant"
DataPath="/mnt/simulations/MoNAsims/baumann"

# Used to allow multiple GEANT runs simultaneously.
UniqueID="_0"

# Make file names based on the file name given.
PreGeant=${DataPath}/${FileName}"-STMONA${UniqueID}.root"
PostGeant=${DataPath}/${FileName}"-GEANT${UniqueID}.root"
FinalName=${DataPath}/${FileName}"_final.root"

# Run the simulations.

# ST MONA simulation.
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}
echo '-------Finished ST MONA-------'

# GEANT afterburner.
# Using light threshold of 0.15 MeVee.
cd ${n2geantpath}/control
control.py -i ${PreGeant} -o ${PostGeant} -t 0.38 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}

# Analysis.
#cd ${stmonapath}
#./bin/mona_analysis -frag ${FRAGname} -if ${PostGeant} -of ${FinalName} # -dT 0.000000001

# Delete intermediate ROOT files.
#cd ${DataPath}
#rm -f ${PreGeant} # ${PostGeant} ${FinalName}
