#!/bin/sh

#First implementation to save time in running simulations.
#Adjust the FileName and the arguments, then save the shell script as a new file.
#Run the shell script to run through all three portions of the simulation.
#This will allow for ease of archival and reproduction of simulations.

P1=$1
P2=$2
#P3=$3
#P4=$4
#P5=$5
printf "Param1 (Ei): ${P1}\n"
printf "Param2 (N):  ${P2}\n"
#printf "Param2 (Ev): ${P2}\n"
#printf "Param3 (SV): ${3}\n"
#printf "Param4 (L): ${P4}\n"
#printf "Param5 (GammaIn): ${P5}\n"


FileName="Be14_He8+2n"

#thermal bckg
#arguments="-exp 09067_He10 -geant -reac 3body_decay -e asymbw ${P1} ${P2} 1 0 -n 100000 -nn 2 -np 2 -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.

#thermal bckg MDJ
arguments="-exp 09067_He10 -geant -reac 2neutron -e1 therm ${P1} -e2 therm ${P1} -n ${P2}  -nn 2 -np 2 -strag 1 -glaub 1.0 -velocityshift 0.97 -asymMomentum 1.13" #-n number of events.


#for geant simulation through python script control.py (-n number of neutrons)
#controlargs="-n 2 -m manual -s vison"
controlargs="-n 2"

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
inputpath="/projects/mona-sim/jonesm/st_mona/input_files"
n2geantpath="/projects/mona-sim/jonesm/n2_geant"
#DataPath="/mnt/simulations/MoNA_EfficiencyTest/Be14_He10/fine_mesh"

#used to allow multiple geant runs simultaneously 
#UniqueID="_Volya_Ei${P1}_Ev${P2}_SV${P3}_L${P4}_Gin${P5}"
UniqueID="_T${P1}MeV"
#UniqueID="_test_GS"
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


#Set Input File Parameters
# A list of available parameters can be found in st_mona.c, do not forget that mona_analysis reads the same input file 
# and also contains the same list. If you wish to add additional parameters, you must change the code in BOTH. 
# The way the code is set up, the "=" serves as a delimiter between variable and value, and the ";" denotes the end of a line.
# Hence, formatting matters here!
# It is important that the CosyMap have quotes, it tells st_mona that it is a string.
# NOTE: These values are set BEFORE st_mona arguments are read. Therefore, if you have inconsistent values the arguments 
#       listed earlier WILL TAKE PRIORITY.

declare -a parameters=( "eBeam          = 59.34;" 
                        "beamA          = 14;" 
                        "beamZ          = 4;" 
                        "dTarget        = 435.0;"
                        "dEbeam         = 0.0215;"
                        "nNeutr         = 2;"
                        "nProt          = 2;"
                        "bSpotCx        = 0.0;"
                        "bSpotCtx       = 0.0;"
                        "bSpotCy        = 0.0;"
                        "bSpotCty       = 0.0;"
                        "bSpotDx        = 0.003;"
                        "bSpotDy        = 0.0045;"
                        "bSpotDtx       = 0.006;"
                        "bSpotDty       = 0.00325;"
                        "crdc1MaskLeft  = 0.15;"
                        "crdc1MaskRight = -0.15;"
                        "crdc2dist      =  1.88;"
                        "monaDist       =   8.2;"
                        "cosymap        = \"m8he_jesse\";"
                        "maxbulge       = 0.000000000005;" 
                        "tflatfoil      = 0.000000125;"
                        "fragA          = 8.0;"
                        "fragZ          = 2.0;"
                        "targA          = 9.0;"
                        "targZ          = 4.0;")

#Write the input file, the first argument is the name of the copy file
#There's most certainly a more elegant way to do this, but its Friday afternoon....
cd ${inputpath}
sh write_input.sh "test_input.txt" "${parameters[0]}" "${parameters[1]}" "${parameters[2]}" "${parameters[3]}" "${parameters[4]}" "${parameters[5]}" "${parameters[6]}" "${parameters[7]}" "${parameters[8]}" "${parameters[9]}" "${parameters[10]}" "${parameters[11]}" "${parameters[12]}" "${parameters[13]}" "${parameters[14]}" "${parameters[15]}" "${parameters[16]}" "${parameters[17]}" "${parameters[18]}" "${parameters[19]}" "${parameters[20]}" "${parameters[21]}" "${parameters[22]}" "${parameters[23]}" "${parameters[24]}" "${parameters[25]}" 

#Run the simulations.

#st_mona simulation
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}

#geant afterburner
#Using light threshold of 0.15 MeVee.
cd ${n2geantpath}/control
##control.py -i ${PreGeant} -o ${PostGeant} -t 0.15 ${controlargs} -u ${UniqueID} -d ${DataPath} -c ${MoNADetFile}
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
rm ${PreGeant} ${PostGeant} ${FinalName}
