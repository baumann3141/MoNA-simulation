#!/bin/bash

#awk -v IGNORECASE=1 -f <(
#  P=; for k; 
#  do [ -z "$P" ] && P="/$k/" || P="$P&&/$k/"; 
#  LINE=$k
#  done
#  echo "$P{print}"
#  echo "$LINE"
# First input is maxbulge in [mm]
# Please document other changes
bulge=$1 #take bulge as input
line=`grep -n "maxbulge" input.txt | cut -f1 -d:` #finds the line number with the term "maxbulge"
sed -i "${line}s/.*/maxbulge 	= ${bulge};/g" input.txt #replace ${line} with new string

