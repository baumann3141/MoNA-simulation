#!/bin/sh
FileName="12B-3p"

arguments="-n 100000 -f root -geant -reac glaub -e asymbw 1 1 0 0 -be 43.94031 -bZ 5 -bA 12 -glaub 1 -strag 0 -nn 0 -np 3"

# For GEANT simulation through Python script control.py (-n number of neutrons).
controlargs="-n 1"

# Fragment for mona_analysis.cc.
FRAGname="He8_12B_e15091"

# MoNA detector configuration file.
MoNADetFile="detector_config"

# Change the random seed if long runs are needed, but you want to do them as several short runs.
export GSL_RNG_SEED=5719

#export MENATEG4XS="/projects/mona-sim/hope_geant/MENATE_R_CrossSections/"

# Paths to access the executables, and to save the data files.
analysispath="/projects/mona-sim/votaw_stmona"
stmonapath="/projects/mona-sim/votaw_stmona/st_mona"
n2geantpath="/projects/mona-sim/votaw_stmona/n2_geant"
DataPath="/mnt/analysis/e15091/simulations"

# Used to allow multiple GEANT runs simultaneously.
UniqueID="_0"

# Make file names based on the file name given.
PreGeant=${DataPath}/${FileName}"-STMONA${UniqueID}.root"
PostGeant=${DataPath}/${FileName}"-GEANT${UniqueID}.root"
FinalName=${DataPath}/${FileName}"_final.root"
#HistDumpName=${DataPath}/${FileName}"-hist${UniqueID}.root"

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
cd ${stmonapath}
./bin/mona_analysis -frag ${FRAGname} -if ${PostGeant} -of ${FinalName} # -dT 5.0

# Dump Edecay histograms.
# Needed for running large jobs and minimzing space used.
#cd ${analysispath}
#root -q -b 'DumpHistsMoNASim.C("'${HistDumpName}'","'${PostGeant}'","'${FinalName}'")'

# Delete intermediate ROOT files.
cd ${DataPath}
rm -f ${PreGeant} # ${PostGeant} ${FinalName}
