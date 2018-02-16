#!/bin/bash

for ((num1=-1; num1<3; num1++))
do

for ((num2=-1; num2<2; num2++)) 
do

for ((num3=-15; num3<16; num3++))
do

### 11Be:
FileName="11Be_opt"
arguments="-n 100000 -f root -be 43.2733 -bZ 4 -bA 11 -nn 0 -np 0 -cym ${num1} -ctym ${num2} -sytym ${num3}"
FRAGname="11Be_e15091"

### 12B:
#FileName="12B_opt"
#arguments="-n 100000 -f root -be 43.94031 -bZ 5 -bA 12 -nn 0 -np 0 -cym ${num1} -ctym ${num2} -sytym ${num3}"
#FRAGname="12B_e15091"

controlargs="-n 0"
MoNADetFile="detector_config"
export GSL_RNG_SEED=5719
analysispath="/projects/mona-sim/votaw_stmona"
stmonapath="/projects/mona-sim/votaw_stmona/st_mona"
n2geantpath="/projects/mona-sim/votaw_stmona/n2_geant"
DataPath="/mnt/analysis/e15091/simulations"
UniqueID="_Cy_${num1}_Cty_${num2}_Syty_${num3}"
PreGeant=${DataPath}/${FileName}"${UniqueID}.root"
cd ${stmonapath}
./bin/st_mona $arguments -f ${PreGeant}

done

done

done
