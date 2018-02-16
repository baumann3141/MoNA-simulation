#!/bin/bash
#Input is MoNA_RADIUS MoNA_ANGLE LISA1_RADIUS LISA1_ANGLE LISA2_RADIUS LISA2_ANGLE
#Units are in [cm] and [deg]

NUMBAR=16
BAR="MoNABar"

#dR = space inbetween bars in [cm]
dR=10
MoNA_RADIUS=$1
LISA1_RADIUS=$3
LISA2_RADIUS=$5

PI=`echo "4*a(1)" | bc -l`

MoNA_ANGLE=`echo "$2*($PI/180)" | bc -l`
MoNA_ANGLE=${MoNA_ANGLE:0:5}

LISA1_ANGLE=`echo "$4*($PI/180)" | bc -l`
LISA1_ANGLE=${LISA1_ANGLE:0:5}

LISA2_ANGLE=`echo "$6*($PI/180)" | bc -l`
LISA2_ANGLE=${LISA2_ANGLE:0:5}


#Define facing vectors
MoNA_FACING_VECTOR="$MoNA_ANGLE 0.0 -1.0 00.0"
LISA1_FACING_VECTOR="$LISA1_ANGLE 0.0 -1.0 00.0"
LISA2_FACING_VECTOR="$LISA2_ANGLE 0.0 -1.0 00.0"

#Generate Initial MoNA Coordinates (Center of front bar)
#dX,dY,dZ are incrimental changes in XYZ in lab frame
MoNA_X=`echo "$1*s($MoNA_ANGLE)" | bc -l`
MoNA_dX=`echo "$dR*s($MoNA_ANGLE)" | bc -l`

#I truncate here because nobody likes 20 digits
#MoNA_X=${MoNA_X:0:5}
#MoNA_dX=${MoNA_dX:0:5}

MoNA_Y=0.0

MoNA_Z=`echo "$1*c($MoNA_ANGLE)" | bc -l`
MoNA_dZ=`echo "$dR*c($MoNA_ANGLE)" | bc -l`

#MoNA_Z=${MoNA_Z:0:5}
#MoNA_dZ=${MoNA_dZ:0:5}

#Generate Initial LISA1 Coordinates (Center of front bar)
LISA1_X=`echo "$3*s($LISA1_ANGLE)" | bc -l`
LISA1_dX=`echo "$dR*s($LISA1_ANGLE)" | bc -l`

#LISA_X=${LISA_X:0:5}
#LISA_dX=${LISA_dX:0:5}

LISA1_Y=0.0

LISA1_Z=`echo "$3*c($LISA1_ANGLE)" | bc -l`
LISA1_dZ=`echo "$dR*c($LISA1_ANGLE)" | bc -l`

#LISA_Z=${LISA_Z:0:5}
#LISA_dZ=${LISA_dZ:0:5}

#Generate Initial LISA2 Coordinates (Center of front bar)
LISA2_X=`echo "$5*s($LISA2_ANGLE)" | bc -l`
LISA2_dX=`echo "$dR*s($LISA2_ANGLE)" | bc -l`

#LISA_X=${LISA_X:0:5}
#LISA_dX=${LISA_dX:0:5}

LISA2_Y=0.0

LISA2_Z=`echo "$5*c($LISA2_ANGLE)" | bc -l`
LISA2_dZ=`echo "$dR*c($LISA2_ANGLE)" | bc -l`


#Define position vector(s)

MoNA_POSITION_VECTOR="$MoNA_X $MoNA_Y $MoNA_Z"
LISA1_POSITION_VECTOR="$LISA1_X $LISA1_Y $LISA1_Z"
LISA2_POSITION_VECTOR="$LISA2_X $LISA2_Y $LISA2_Z"


#Create new configuration file
echo -n "" > /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
echo "!target position (X Y Z in cm)" >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
echo "0. 0. 0." >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
echo "!MoNA" >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1

#Build MoNA
for WALL in {A..I}
do
  
   echo "$MoNA_POSITION_VECTOR $NUMBAR $MoNA_FACING_VECTOR $BAR $WALL" >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
   MoNA_X=`echo "$MoNA_X+$MoNA_dX" |bc -l`
   MoNA_Z=`echo "$MoNA_Z+$MoNA_dZ" |bc -l`
   MoNA_POSITION_VECTOR="$MoNA_X $MoNA_Y $MoNA_Z"

done

#Build LISA

for WALL in {J..M}
do
  
   echo "$LISA1_POSITION_VECTOR $NUMBAR $LISA1_FACING_VECTOR $BAR $WALL" >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
   LISA1_X=`echo "$LISA1_X+$LISA1_dX" |bc -l`
   LISA1_Z=`echo "$LISA1_Z+$LISA1_dZ" |bc -l`
   LISA1_POSITION_VECTOR="$LISA1_X $LISA1_Y $LISA1_Z"
     #Gap in LISA tables, 75 cm
     #if [ "$WALL" == "L" ] 
     #   then 
     #        GAP_dX=`echo "7.5*$dR*s($LISA_ANGLE)" | bc -l`
     #        GAP_dZ=`echo "7.5*$dR*c($LISA_ANGLE)" | bc -l`
     #        LISA_X=`echo "$LISA_X+$GAP_dX" |bc -l`
     #        LISA_Z=`echo "$LISA_Z+$GAP_dZ" |bc -l`
     #fi
   

done

for WALL in {N..R}
do
  
   echo "$LISA2_POSITION_VECTOR $NUMBAR $LISA2_FACING_VECTOR $BAR $WALL" >> /projects/mona-sim/baumann/single_target/n2_geant/detector_config_temp1
   LISA2_X=`echo "$LISA2_X+$LISA2_dX" |bc -l`
   LISA2_Z=`echo "$LISA2_Z+$LISA2_dZ" |bc -l`
   LISA2_POSITION_VECTOR="$LISA2_X $LISA2_Y $LISA2_Z"

done




