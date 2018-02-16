#!/bin/bash

rm CycSrimDict.cxx

make CycSrimDict.cxx

make clean

make

cp -f CycSrimDict_rdict.pcm ../bin/.

cp -f monaAnaDict_rdict.pcm ../bin/.

echo "ST MONA compilation complete."

cd ../../n2_geant/control/

./Makefile

echo "N2 GEANT compilation complete."
